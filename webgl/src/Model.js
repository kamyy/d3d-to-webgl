import { getCurrScene } from './store/appStore.js'
import RefFrame from './RefFrame.js'
import { GL } from './App.js'

export default class Model extends RefFrame {
  constructor(parent, node, scene) {
    super(parent, node)

    this.shaderP3C3 = GL.mapOfShaders.get('P3C3')
    this.isTheFloor = false
    this.scene = scene

    if ('pieces' in node) {
      this.modelPieces = node.pieces
      for (let piece of this.modelPieces) {
        const nameOfMaterial = piece.material

        if (nameOfMaterial === 'floor') {
          this.isTheFloor = true
        }
        piece.material = scene.mapOfMaterials.get(nameOfMaterial)
        piece.nrmBuffer = GL.createBuffer()
        piece.vtxBuffer = GL.createBuffer()
        piece.idxBuffer = GL.createBuffer()

        GL.bindBuffer(GL.ARRAY_BUFFER, piece.nrmBuffer)
        GL.bufferData(GL.ARRAY_BUFFER, new Float32Array(piece.nrms), GL.STATIC_DRAW)

        GL.bindBuffer(GL.ARRAY_BUFFER, piece.vtxBuffer)
        GL.bufferData(GL.ARRAY_BUFFER, new Float32Array(piece.vtxs), GL.STATIC_DRAW)

        GL.bindBuffer(GL.ELEMENT_ARRAY_BUFFER, piece.idxBuffer)
        GL.bufferData(GL.ELEMENT_ARRAY_BUFFER, new Uint16Array(piece.idxs), GL.STATIC_DRAW)

        piece.nrmVtxCount = piece.nrms.length / this.shaderP3C3.constructor.vertexElementCount
        piece.triVtxCount = piece.idxs.length
      }
    } else {
      this.modelPieces = []
    }
  }

  drawNormals() {
    const currScene = getCurrScene()

    this.modelPieces.forEach((piece) => {
      if (currScene.filteredMaterials.includes(piece.material)) {
        this.shaderP3C3.drawNormals(this, piece)
      }
    })
  }

  drawPieces(forReflection, cacheTranslucentPiece) {
    const currScene = getCurrScene()

    this.modelPieces.forEach((piece) => {
      if (currScene.filteredMaterials.includes(piece.material)) {
        if (!forReflection && piece.material.isTranslucent) {
          cacheTranslucentPiece({ model: this, piece: piece })
        } else {
          piece.material.shader.drawTriangles(this, piece)
        }
      }
    })
  }

  drawEdges() {
    const currScene = getCurrScene()

    this.modelPieces.forEach((piece) => {
      if (currScene.filteredMaterials.includes(piece.material)) {
        piece.material.shader.drawTriangleEdges(this, piece)
      }
    })
  }
}
