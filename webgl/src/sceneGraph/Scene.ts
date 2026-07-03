import { GL, rawScenes } from "../app/App"
import { sceneLoaded, type SceneState } from "../redux/sceneSlice"
import store, { getCurrScene } from "../redux/store"

import Model from "./Model"
import Camera from "./Camera"
import RefFrame from "./RefFrame"
import OmniDirLS from "./OmniDirLS"
import Matrix4x4 from "../math/Matrix4x4"
import { Material, MaterialJson, SceneJson, SceneNodeJson, TextureJson, TranslucentPieceEntry } from "../types/scene"
import { ShaderProgramName } from "../types/webgl"
import Shader from "../shader/Shader"

const DRAW = Object.freeze({
  MIRROR: Symbol("mirror"),
  PIECES: Symbol("pieces"),
})

type DrawMode = typeof DRAW.MIRROR | typeof DRAW.PIECES

async function fetchJson<T>(url: string): Promise<T> {
  const response = await fetch(url)
  if (!response.ok) {
    throw new Error(`Cannot GET ${url} status=${response.status}`)
  }
  return response.json() as Promise<T>
}

export default class Scene {
  id: number
  name: string
  cameras: Camera[]
  translucentPieces: TranslucentPieceEntry[]
  sceneLoadRequired: boolean
  mapOfTextures!: Map<string, WebGLTexture>
  mapOfMaterials!: Map<string, Material>
  rootNode!: RefFrame
  omniDirLS!: OmniDirLS
  mirrorCam!: Camera
  mirrorObj!: Model

  constructor(id: number, name: string) {
    this.id = id
    this.name = name
    this.cameras = []
    this.translucentPieces = []
    this.sceneLoadRequired = true

    this.drawScene = this.drawScene.bind(this)
    this.cacheTranslucentPiece = this.cacheTranslucentPiece.bind(this)
  }

  loadScene(): void {
    if (GL && this.sceneLoadRequired) {
      this.sceneLoadRequired = false

      void (async () => {
        try {
          const json = await fetchJson<SceneJson>(`/json/${this.name}.json`)
          const id = this.id

          await this.initTextures(json.textures)
          this.initMaterials(json.materials)
          this.initSceneRoot(json.sceneRoot)

          store.dispatch(sceneLoaded({ id, scene: this }))
          const currScene = getCurrScene()

          if (currScene && currScene.id === id) {
            rawScenes[id].requestDrawScene()
          }
        } catch (error) {
          console.error(`Failed to GET /json/${this.name}.json`, error)
        }
      })()
    }
  }

  initTextures(textures: TextureJson[]): Promise<void[]> {
    if (!GL) {
      return Promise.resolve([])
    }
    const gl = GL

    this.mapOfTextures = new Map()

    const promises = textures.map((tex) => {
      const image = new Image()
      const glTex = gl.createTexture() as WebGLTexture

      gl.bindTexture(gl.TEXTURE_2D, glTex)
      gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, 1, 1, 0, gl.RGBA, gl.UNSIGNED_BYTE, new Uint8Array([0, 0, 255, 255]))

      this.mapOfTextures.set(tex.name, glTex)
      return new Promise<void>((resolve) => {
        image.onload = () => {
          gl.bindTexture(gl.TEXTURE_2D, this.mapOfTextures.get(tex.name) as WebGLTexture)
          if (tex.hasAlpha) {
            gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, image)
          } else {
            gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, image)
          }
          gl.generateMipmap(gl.TEXTURE_2D)
          resolve()
        }
        image.src = `/textures/${tex.name}.png`
      })
    })

    return Promise.all(promises)
  }

  initMaterials(materials: MaterialJson[]): void {
    if (!GL) {
      return
    }

    this.mapOfMaterials = new Map()

    for (const mat of materials) {
      const material = { ...mat } as Material
      this.mapOfMaterials.set(mat.name, material)
      material.textures = mat.textures.map((texName) => (texName ? (this.mapOfTextures.get(texName) ?? null) : null))
      material.shader = GL.mapOfShaders.get(mat.shaderProgram as ShaderProgramName) as Shader
    }
  }

  initSceneRoot(node: SceneNodeJson): void {
    this.rootNode = new RefFrame(null, node)

    if (node.children) {
      for (const child of node.children) {
        this.initSceneGraph(child, this.rootNode)
      }
    }
  }

  initSceneGraph(node: SceneNodeJson, parent: RefFrame): void {
    let refFrame: RefFrame

    switch (node.nodeType) {
      case "OmniDirLS":
        refFrame = new OmniDirLS(parent, node)
        this.omniDirLS = refFrame as OmniDirLS
        break

      case "Camera":
        refFrame = new Camera(parent, node)
        this.cameras.push(refFrame as Camera)
        break

      case "MirrorCamera":
        refFrame = new Camera(parent, node)
        this.mirrorCam = refFrame as Camera
        break

      case "Model":
        refFrame = new Model(parent, node, this)
        if ((refFrame as Model).isTheFloor) {
          this.mirrorObj = refFrame as Model
        }
        break

      default:
        refFrame = new RefFrame(parent, node)
        break
    }

    if (node.children) {
      node.children.forEach((child) => this.initSceneGraph(child, refFrame))
    }
  }

  requestDrawScene(): void {
    requestAnimationFrame(this.drawScene)
  }

  drawScene(): void {
    const currScene = getCurrScene()

    if (currScene && currScene.id === this.id && GL) {
      GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT | GL.STENCIL_BUFFER_BIT)

      this.activeCamera.fieldOfView = currScene.cameras[this.activeCamIdx].fieldOfView
      this.activeCamera.aspectRatio = currScene.cameras[this.activeCamIdx].aspectRatio

      if (this.mirrorObj) {
        GL.disable(GL.BLEND)

        GL.enable(GL.STENCIL_TEST)
        GL.stencilFunc(GL.ALWAYS, 1, 1)
        GL.stencilOp(GL.KEEP, GL.INCR, GL.INCR)
        GL.depthMask(false)
        GL.colorMask(false, false, false, false)

        this.mirrorObj.drawPieces(1, this.cacheTranslucentPiece)

        GL.cullFace(GL.FRONT)
        GL.stencilFunc(GL.EQUAL, 1, 1)
        GL.stencilOp(GL.KEEP, GL.KEEP, GL.KEEP)
        GL.depthMask(true)
        GL.colorMask(true, true, true, true)

        let m = new Matrix4x4(this.activeCamera.modelMatrix)
        m = m.mul(this.mirrorObj.modelMatrix.inverse())
        m = m.postCatSxyz(1, 1, -1)
        m = m.mul(this.mirrorObj.modelMatrix)

        this.mirrorCam.fieldOfView = this.activeCamera.fieldOfView
        this.mirrorCam.aspectRatio = this.activeCamera.aspectRatio
        this.mirrorCam.clipDistanceN = this.activeCamera.clipDistanceN
        this.mirrorCam.clipDistanceF = this.activeCamera.clipDistanceF

        const savedCamera = this.activeCamera
        this.activeCamera = this.mirrorCam
        this.activeCamera.modelMatrix = m

        this.drawNode(this.rootNode, DRAW.MIRROR, currScene)

        this.activeCamera = savedCamera

        GL.clear(GL.DEPTH_BUFFER_BIT)
        GL.disable(GL.STENCIL_TEST)
        GL.cullFace(GL.BACK)
        GL.enable(GL.BLEND)

        if (currScene.drawWireframe) {
          this.mirrorObj.drawEdges()
        } else {
          this.mirrorObj.drawPieces(1, this.cacheTranslucentPiece)
        }
      }

      this.drawNode(this.rootNode, DRAW.PIECES, currScene)
      this.drawTranslucentPieces()
    }
  }

  drawNode(node: RefFrame | null, mode: DrawMode, currScene: SceneState): void {
    if (node) {
      if (node instanceof Model && node !== this.mirrorObj) {
        switch (mode) {
          case DRAW.MIRROR:
            if (currScene.drawWireframe) {
              node.drawEdges()
            } else {
              node.drawPieces(1, this.cacheTranslucentPiece)
            }
            break

          case DRAW.PIECES:
            if (currScene.drawWireframe) {
              node.drawEdges()
            } else {
              node.drawPieces(0, this.cacheTranslucentPiece)
            }
            if (currScene.drawNormals) {
              node.drawNormals()
            }
            break
        }
      }

      for (const child of node.children()) {
        this.drawNode(child, mode, currScene)
      }
    }
  }

  drawTranslucentPieces(): void {
    this.translucentPieces.forEach(({ model, piece }) => {
      piece.material.shader.drawTriangles(model, piece)
    })
    this.translucentPieces.length = 0
  }

  cacheTranslucentPiece(piece: TranslucentPieceEntry): void {
    this.translucentPieces.push(piece)
  }

  set activeCamera(cam: Camera) {
    this.cameras[this.activeCamIdx] = cam
  }

  get activeCamera(): Camera {
    return this.cameras[this.activeCamIdx]
  }

  get activeCamIdx(): number {
    return store.getState().allScenes[this.id]?.cameraIdx ?? 0
  }
}
