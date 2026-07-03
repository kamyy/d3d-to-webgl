import { createSlice, type PayloadAction } from "@reduxjs/toolkit"
import type Scene from "../sceneGraph/Scene"
import type { Material } from "../types/scene"
import type { RootState } from "./store"

export interface CameraState {
  fieldOfView: number
  aspectRatio: number
}

export type LightSourceType = "Omni Directional" | "Lower Ambient" | "Upper Ambient"

export interface SceneState {
  id: number
  cameras: CameraState[]
  cameraIdx: number
  currLSType: LightSourceType
  omniDirRGB: [number, number, number]
  lowerAmbientRGB: [number, number, number]
  upperAmbientRGB: [number, number, number]
  drawWireframe: boolean
  drawNormals: boolean
  materialFilter: string
  originalMaterials: Material[]
  filteredMaterials: Material[]
}

interface SceneStoreState {
  allScenes: (SceneState | null)[]
  currIndex: number
}

type LightColorChannel = 0 | 1 | 2

interface LightColorChangedPayload {
  channel: LightColorChannel
  value: number
}

const initialState: SceneStoreState = {
  allScenes: [null, null, null],
  currIndex: 0,
}

function getCurrScene(state: SceneStoreState) {
  return state.allScenes[state.currIndex]
}

function getCurrLightRGB(scene: SceneState) {
  switch (scene.currLSType) {
    case "Omni Directional":
      return scene.omniDirRGB
    case "Lower Ambient":
      return scene.lowerAmbientRGB
    case "Upper Ambient":
      return scene.upperAmbientRGB
  }
}

function getCurrLightRGBChannel(state: RootState, channel: LightColorChannel) {
  const scene = getCurrScene(state)
  return scene ? getCurrLightRGB(scene)[channel] : undefined
}

const sceneSlice = createSlice({
  name: "scene",

  initialState,

  reducers: {
    sceneLoaded(state, { payload: { id, scene } }: PayloadAction<{ id: number; scene: Scene }>) {
      const { cameras, omniDirLS, mapOfMaterials } = scene
      state.allScenes[id] = {
        id,
        cameras: cameras.map((cam) => ({ fieldOfView: cam.fieldOfView, aspectRatio: cam.aspectRatio })),
        cameraIdx: 0,
        currLSType: "Omni Directional",
        omniDirRGB: [...omniDirLS.color],
        lowerAmbientRGB: [0.2, 0.2, 0.2],
        upperAmbientRGB: [0.2, 0.2, 0.2],
        drawWireframe: false,
        drawNormals: false,
        materialFilter: "",
        originalMaterials: Array.from(mapOfMaterials.values()),
        filteredMaterials: Array.from(mapOfMaterials.values()),
      }
    },

    sceneChanged(state, { payload: id }: PayloadAction<number>) {
      state.currIndex = id
    },

    cameraChanged(state, { payload: cameraIdx }: PayloadAction<number>) {
      const scene = getCurrScene(state)
      if (scene) {
        scene.cameraIdx = cameraIdx
      }
    },

    fieldOfViewChanged(state, { payload: fieldOfView }: PayloadAction<number>) {
      const scene = getCurrScene(state)
      if (scene) {
        scene.cameras[scene.cameraIdx].fieldOfView = fieldOfView
      }
    },

    aspectRatioChanged(state, { payload: aspectRatio }: PayloadAction<number>) {
      const scene = getCurrScene(state)
      if (scene) {
        scene.cameras[scene.cameraIdx].aspectRatio = aspectRatio
      }
    },

    lightSourceChanged(state, { payload: lightSource }: PayloadAction<LightSourceType>) {
      const scene = getCurrScene(state)
      if (scene) {
        scene.currLSType = lightSource
      }
    },

    lightColorChannelChanged(state, { payload: { channel, value } }: PayloadAction<LightColorChangedPayload>) {
      const scene = getCurrScene(state)
      if (scene) {
        getCurrLightRGB(scene)[channel] = value
      }
    },

    wireframeToggled(state) {
      const scene = getCurrScene(state)
      if (scene) {
        scene.drawWireframe = !scene.drawWireframe
      }
    },

    normalsToggled(state) {
      const scene = getCurrScene(state)
      if (scene) {
        scene.drawNormals = !scene.drawNormals
      }
    },

    materialFilterChanged(state, { payload: filter }: PayloadAction<string>) {
      const scene = getCurrScene(state)
      if (scene) {
        const lcFilter = filter.toLowerCase()
        const filterFn = (material: { name: string }) => material.name.toLowerCase().includes(lcFilter)

        scene.materialFilter = filter
        scene.filteredMaterials = scene.originalMaterials.filter(filterFn)
      }
    },
  },
})

export default sceneSlice.reducer

export const {
  sceneLoaded,
  sceneChanged,
  cameraChanged,
  fieldOfViewChanged,
  aspectRatioChanged,
  lightSourceChanged,
  lightColorChannelChanged,
  wireframeToggled,
  normalsToggled,
  materialFilterChanged,
} = sceneSlice.actions

export function selectCurrScene(state: RootState) {
  return getCurrScene(state)
}

export function selectCameraIdx(state: RootState) {
  return getCurrScene(state)?.cameraIdx
}

export function selectFieldOfView(state: RootState) {
  const scene = getCurrScene(state)
  return scene?.cameras[scene.cameraIdx].fieldOfView
}

export function selectAspectRatio(state: RootState) {
  const scene = getCurrScene(state)
  return scene?.cameras[scene.cameraIdx].aspectRatio
}

export function selectLightSource(state: RootState) {
  return getCurrScene(state)?.currLSType
}

export function selectLightSourceRed(state: RootState) {
  return getCurrLightRGB(state)[0]
  return getCurrLightRGBChannel(state, 0)
}

export function selectLightSourceGreen(state: RootState) {
  return getCurrLightRGBChannel(state, 1)
}

export function selectLightSourceBlue(state: RootState) {
  return getCurrLightRGBChannel(state, 2)
}

export function selectDrawWireframe(state: RootState) {
  return getCurrScene(state)?.drawWireframe
}

export function selectDrawNormals(state: RootState) {
  return getCurrScene(state)?.drawNormals
}

export function selectMaterialFilter(state: RootState) {
  return getCurrScene(state)?.materialFilter
}

export function selectFilteredMaterials(state: RootState) {
  return getCurrScene(state)?.filteredMaterials
}
