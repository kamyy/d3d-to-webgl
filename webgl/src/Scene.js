import Model from './Model.js'
import Camera from './Camera.js'
import RefFrame from './RefFrame.js'
import OmniDirLS from './OmniDirLS.js'
import Matrix4x4 from './Matrix4x4.js'

import { GL, reduxStore, sceneArray } from './App'
import { actionCreators } from './Actions'

const DRAW = Object.freeze({
  MIRROR: Symbol('mirror'),
  PIECES: Symbol('pieces'),
})

function createXHR(url, mimeType = null) {
  return new Promise(function (resolve, reject) {
    const xhr = new XMLHttpRequest()
    if (mimeType) {
      xhr.overrideMimeType(mimeType)
    }
    xhr.onreadystatechange = function () {
      if (this.readyState === 4) {
        switch (this.status) {
          case 200:
            resolve(this.responseText)
            break
          default:
            reject(this.status)
            break
        }
      }
    }
    xhr.open('GET', url, true)
    xhr.send()
  })
}

export default class Scene {
  constructor(id, name) {
    this.id = id
    this.name = name
    this.cameras = []
    this.translucentPieces = []
    this.sceneLoadRequired = true

    this.drawScene = this.drawScene.bind(this)
    this.cacheTranslucentPiece = this.cacheTranslucentPiece.bind(this)
  }

  loadScene() {
    if (GL && this.sceneLoadRequired) {
      this.sceneLoadRequired = false

      createXHR(`/json/${this.name}.json`, 'application/json')
        .then(async (responseText) => {
          const json = JSON.parse(responseText)

          await this.initTextures(json.textures)
          this.initMaterials(json.materials)
          this.initSceneRoot(json.sceneRoot)
          reduxStore.dispatch(actionCreators.onSceneLoad(this.id, this))

          const scene = sceneArray.curScene
          if (scene.id === this.id) {
            scene.requestDrawScene()
          }
        })
        .catch((status) => {
          console.error(`Failed to GET /json/${this.name}.json status(${status})`)
        })
    }
  }

  initTextures(textures) {
    this.mapOfTextures = new Map()

    const promises = textures.map((tex) => {
      const image = new Image()
      const glTex = GL.createTexture()

      GL.bindTexture(GL.TEXTURE_2D, glTex)
      GL.texImage2D(GL.TEXTURE_2D, 0, GL.RGBA, 1, 1, 0, GL.RGBA, GL.UNSIGNED_BYTE, new Uint8Array([0, 0, 255, 255]))

      this.mapOfTextures.set(tex.name, glTex)
      return new Promise((resolve) => {
        image.onload = () => {
          GL.bindTexture(GL.TEXTURE_2D, this.mapOfTextures.get(tex.name))
          if (tex.hasAlpha) {
            GL.texImage2D(GL.TEXTURE_2D, 0, GL.RGBA, GL.RGBA, GL.UNSIGNED_BYTE, image)
          } else {
            GL.texImage2D(GL.TEXTURE_2D, 0, GL.RGB, GL.RGB, GL.UNSIGNED_BYTE, image)
          }
          GL.generateMipmap(GL.TEXTURE_2D)
          resolve()
        }
        image.src = `/textures/${tex.name}.png`
      })
    })

    return Promise.all(promises)
  }

  initMaterials(materials) {
    this.mapOfMaterials = new Map()

    for (let mat of materials) {
      this.mapOfMaterials.set(mat.name, mat)
      mat.textures.forEach((tex, i) => {
        if (tex) {
          mat.textures[i] = this.mapOfTextures.get(tex)
        }
      })
      mat.shader = GL.mapOfShaders.get(mat.shaderProgram)
    }
  }

  initSceneRoot(node) {
    this.rootNode = new RefFrame(null, node)

    if (node.hasOwnProperty('children')) {
      for (let child of node.children) {
        this.initSceneGraph(child, this.rootNode)
      }
    }
  }

  initSceneGraph(node, parent) {
    let refFrame

    switch (node.nodeType) {
      case 'OmniDirLS':
        refFrame = new OmniDirLS(parent, node)
        this.omniDirLS = refFrame
        break

      case 'Camera':
        refFrame = new Camera(parent, node)
        this.cameras.push(refFrame)
        break

      case 'MirrorCamera':
        refFrame = new Camera(parent, node)
        this.mirrorCam = refFrame
        break

      case 'Model':
        refFrame = new Model(parent, node, this)
        if (refFrame.isTheFloor) {
          this.mirrorObj = refFrame
        }
        break

      default:
        refFrame = new RefFrame(parent, node)
        break
    }

    if (node.hasOwnProperty('children')) {
      node.children.forEach((child) => this.initSceneGraph(child, refFrame))
    }
  }

  requestDrawScene() {
    requestAnimationFrame(this.drawScene)
  }

  drawScene() {
    const { sceneArray, curSceneId } = reduxStore.getState()

    const sceneState = sceneArray[this.id]

    if (sceneState && curSceneId === this.id) {
      GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT | GL.STENCIL_BUFFER_BIT)

      this.activeCamera.fieldOfView = sceneState.cameras[this.activeCamIdx].fieldOfView
      this.activeCamera.aspectRatio = sceneState.cameras[this.activeCamIdx].aspectRatio

      if (this.mirrorObj) {
        GL.disable(GL.BLEND) // disable alpha blending

        GL.enable(GL.STENCIL_TEST) // enable stencil buffer
        GL.stencilFunc(GL.ALWAYS, 1, 1) // stencil test always passes
        GL.stencilOp(
          GL.KEEP, // if stencil test fail do nothing
          GL.INCR, // if stencil test pass depth test fail write 1 to stencil
          GL.INCR
        ) // if stencil test pass depth test pass write 1 to stencil
        GL.depthMask(false) // disable depth buffer writes
        GL.colorMask(false, false, false, false) // disable color buffer writes

        this.mirrorObj.drawPieces(1, this.cacheTranslucentPiece) // draw mirror into stencil

        GL.cullFace(GL.FRONT) // cull CW triangles
        GL.stencilFunc(GL.EQUAL, 1, 1) // stencil test pass if stencil == 1
        GL.stencilOp(
          GL.KEEP, // if stencil test fail do nothing
          GL.KEEP, // if stencil test pass depth test fail do nothing
          GL.KEEP
        ) // if stencil test pass depth test pass do nothing
        GL.depthMask(true) // enable depth buffer writes
        GL.colorMask(true, true, true, true) // enable color buffer writes

        // generate reflection matrix using mirror model matrix and scaling by -1 in z-axis
        let m = new Matrix4x4(this.activeCamera.modelMatrix)
        m = m.mul(this.mirrorObj.modelMatrix.inverse()) // world space to mirror space
        m = m.postCatSxyz(1, 1, -1) // to mirror reflection space
        m = m.mul(this.mirrorObj.modelMatrix) // to world space xform

        // copy active camera settings to mirror camera
        this.mirrorCam.fieldOfView = this.activeCamera.fieldOfView
        this.mirrorCam.aspectRatio = this.activeCamera.aspectRatio
        this.mirrorCam.clipDistanceN = this.activeCamera.clipDistanceN
        this.mirrorCam.clipDistanceF = this.activeCamera.clipDistanceF

        const savedCamera = this.activeCamera // restore this after rendering reflection
        this.activeCamera = this.mirrorCam // make mirror camera rendering camera
        this.activeCamera.modelMatrix = m // apply mirror xform to mirror camera

        this.drawNode(this.rootNode, DRAW.MIRROR, sceneState) // draw reflection of model pieces into color buffer
        this.activeCamera = savedCamera // restore default camera

        GL.clear(GL.DEPTH_BUFFER_BIT) // clear depth buffer
        GL.disable(GL.STENCIL_TEST) // disable stencil buffer
        GL.cullFace(GL.BACK) // cull CCW triangles
        GL.enable(GL.BLEND) // enable alpha blending

        // draw mirror into color buffer
        sceneState.drawWirefrm ? this.mirrorObj.drawEdges() : this.mirrorObj.drawPieces(1, this.cacheTranslucentPiece)
      }

      this.drawNode(this.rootNode, DRAW.PIECES, sceneState) // draw all models in scene graph
      this.drawTranslucentPieces()
    }
  }

  drawNode(node, mode, sceneState) {
    if (node) {
      if (node instanceof Model && node !== this.mirrorObj) {
        switch (mode) {
          case DRAW.MIRROR:
            sceneState.drawWirefrm ? node.drawEdges() : node.drawPieces(1, this.cacheTranslucentPiece)
            break

          case DRAW.PIECES:
            sceneState.drawWirefrm ? node.drawEdges() : node.drawPieces(0, this.cacheTranslucentPiece)
            if (sceneState.drawNormals) {
              node.drawNormals()
            }
            break

          default:
            break
        }
      }

      for (let child of node.children()) {
        this.drawNode(child, mode, sceneState) // recurse
      }
    }
  }

  drawTranslucentPieces() {
    this.translucentPieces.forEach(({ model, piece }) => {
      piece.material.shader.drawTriangles(model, piece)
    })
    this.translucentPieces.length = 0
  }

  cacheTranslucentPiece(piece) {
    this.translucentPieces.push(piece)
  }

  set activeCamera(cam) {
    this.cameras[this.activeCamIdx] = cam
  }

  get activeCamera() {
    return this.cameras[this.activeCamIdx]
  }

  get activeCamIdx() {
    return reduxStore.getState().sceneArray[this.id].cameraIdx
  }
}
