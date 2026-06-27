import { getCurrScene } from "../store/appStore"
import RefFrame from "./RefFrame"
import { GL } from "../app/App"
import Scene from "./Scene"
import { Material, ModelPiece, SceneNodeJson, TranslucentPieceEntry } from "../types/scene"
import ShaderP3C3 from "../shader/ShaderP3C3"

export default class Model extends RefFrame {
  shaderP3C3: ShaderP3C3
  isTheFloor: boolean
  drawTriangleEdgesOnly: boolean
  scene: Scene
  modelPieces: ModelPiece[]

  constructor(parent: RefFrame, node: SceneNodeJson, scene: Scene) {
    super(parent, node)

    if (!GL) {
      throw new Error("WebGL context not initialized")
    }
    const gl = GL

    this.shaderP3C3 = gl.mapOfShaders.get("P3C3") as ShaderP3C3
    this.isTheFloor = false
    this.drawTriangleEdgesOnly = node.drawTriangleEdgesOnly ?? false
    this.scene = scene

    if (node.pieces) {
      this.modelPieces = node.pieces.map((piece) => {
        const nameOfMaterial = piece.material

        if (nameOfMaterial === "floor" && !this.drawTriangleEdgesOnly) {
          this.isTheFloor = true
        }

        const material = scene.mapOfMaterials.get(nameOfMaterial) as Material
        const modelPiece: ModelPiece = {
          ...piece,
          material,
          nrmBuffer: gl.createBuffer() as WebGLBuffer,
          vtxBuffer: gl.createBuffer() as WebGLBuffer,
          idxBuffer: gl.createBuffer() as WebGLBuffer,
          nrmVtxCount: 0,
          triVtxCount: 0,
        }

        gl.bindBuffer(gl.ARRAY_BUFFER, modelPiece.nrmBuffer)
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(piece.nrms), gl.STATIC_DRAW)

        gl.bindBuffer(gl.ARRAY_BUFFER, modelPiece.vtxBuffer)
        gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(piece.vtxs), gl.STATIC_DRAW)

        gl.bindBuffer(gl.ELEMENT_ARRAY_BUFFER, modelPiece.idxBuffer)
        gl.bufferData(gl.ELEMENT_ARRAY_BUFFER, new Uint16Array(piece.idxs), gl.STATIC_DRAW)

        modelPiece.nrmVtxCount = piece.nrms.length / ShaderP3C3.vertexElementCount
        modelPiece.triVtxCount = piece.idxs.length

        return modelPiece
      })
    } else {
      this.modelPieces = []
    }
  }

  drawNormals() {
    if (!this.drawTriangleEdgesOnly) {
      const currScene = getCurrScene()

      this.modelPieces.forEach((piece) => {
        if (currScene?.filteredMaterials.includes(piece.material)) {
          this.shaderP3C3.drawNormals(this, piece)
        }
      })
    }
  }

  drawPieces(forReflection: number, cacheTranslucentPiece: (entry: TranslucentPieceEntry) => void) {
    if (this.drawTriangleEdgesOnly) {
      this.drawEdges()
      return
    }

    const currScene = getCurrScene()

    this.modelPieces.forEach((piece) => {
      if (currScene?.filteredMaterials.includes(piece.material)) {
        if (!forReflection && piece.material.isTranslucent) {
          cacheTranslucentPiece({ model: this, piece })
        } else {
          piece.material.shader.drawTriangles(this, piece)
        }
      }
    })
  }

  drawEdges(cullBackFacing?: boolean) {
    const currScene = getCurrScene()
    const shouldCullBackFacing = cullBackFacing ?? !this.drawTriangleEdgesOnly

    this.modelPieces.forEach((piece) => {
      if (currScene?.filteredMaterials.includes(piece.material)) {
        piece.material.shader.drawTriangleEdges(this, piece, shouldCullBackFacing)
      }
    })
  }
}
