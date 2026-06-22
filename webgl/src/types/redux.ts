import { Material } from "./scene"

export interface CameraState {
  fieldOfView: number
  aspectRatio: number
}

export type LightSourceType = "Omni Directional" | "Lower Ambient" | "Upper Ambient"

export interface SceneState {
  id: number
  cameras: CameraState[]
  cameraIdx: number
  currentLS: LightSourceType
  omniDirLS: [number, number, number]
  lowerAmbientLS: [number, number, number]
  upperAmbientLS: [number, number, number]
  drawWirefrm: boolean
  drawNormals: boolean
  materialFilter: string
  originalMaterials: Material[]
  filteredMaterials: Material[]
}

export interface RootState {
  allScenes: (SceneState | null)[]
  currIndex: number
}
