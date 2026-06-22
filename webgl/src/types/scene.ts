import Shader from "../shader/Shader"

export interface TextureJson {
  name: string
  hasAlpha: boolean
}

export interface MaterialJson {
  name: string
  diff: [number, number, number]
  spec: [number, number, number]
  shinyExponent: number
  isTranslucent: boolean
  shaderProgram: string
  textures: (string | null)[]
}

export interface ModelPieceJson {
  material: string
  vtxs: number[]
  nrms: number[]
  idxs: number[]
}

export interface SceneNodeJson {
  nodeType?: string
  modelMatrix?: number[]
  children?: SceneNodeJson[]
  pieces?: ModelPieceJson[]
  fieldOfView?: number
  aspectRatio?: number
  clipDistanceN?: number
  clipDistanceF?: number
  color?: [number, number, number]
  coeff0?: number
  coeff1?: number
  coeff2?: number
}

export interface SceneJson {
  textures: TextureJson[]
  materials: MaterialJson[]
  sceneRoot: SceneNodeJson
}

export interface Material extends Omit<MaterialJson, "textures"> {
  textures: (WebGLTexture | null)[]
  shader: Shader
}

export interface ModelPiece {
  material: Material
  vtxs: number[]
  nrms: number[]
  idxs: number[]
  nrmBuffer: WebGLBuffer
  vtxBuffer: WebGLBuffer
  idxBuffer: WebGLBuffer
  nrmVtxCount: number
  triVtxCount: number
}

export interface TranslucentPieceEntry {
  model: import("../Model").default
  piece: ModelPiece
}
