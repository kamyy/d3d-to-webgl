import { createSlice, PayloadAction } from "@reduxjs/toolkit"
import Scene from "../Scene"
import { LightSourceType, RootState } from "../types/redux"

const initialState: RootState = {
  allScenes: [null, null, null],
  currIndex: 0,
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
        currentLS: "Omni Directional",
        omniDirLS: [...omniDirLS.color],
        lowerAmbientLS: [0.2, 0.2, 0.2],
        upperAmbientLS: [0.2, 0.2, 0.2],
        drawWirefrm: false,
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
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        scene.cameraIdx = cameraIdx
      }
    },

    fieldOfViewChanged(state, { payload: fieldOfView }: PayloadAction<number>) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        scene.cameras[scene.cameraIdx].fieldOfView = fieldOfView
      }
    },

    aspectRatioChanged(state, { payload: aspectRatio }: PayloadAction<number>) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        scene.cameras[scene.cameraIdx].aspectRatio = aspectRatio
      }
    },

    lightSourceChanged(state, { payload: lightSource }: PayloadAction<LightSourceType>) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        scene.currentLS = lightSource
      }
    },

    rChanged(state, { payload: value }: PayloadAction<number>) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        switch (scene.currentLS) {
          case "Omni Directional":
            scene.omniDirLS[0] = value
            break
          case "Lower Ambient":
            scene.lowerAmbientLS[0] = value
            break
          case "Upper Ambient":
            scene.upperAmbientLS[0] = value
            break
        }
      }
    },

    gChanged(state, { payload: value }: PayloadAction<number>) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        switch (scene.currentLS) {
          case "Omni Directional":
            scene.omniDirLS[1] = value
            break
          case "Lower Ambient":
            scene.lowerAmbientLS[1] = value
            break
          case "Upper Ambient":
            scene.upperAmbientLS[1] = value
            break
        }
      }
    },

    bChanged(state, { payload: value }: PayloadAction<number>) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        switch (scene.currentLS) {
          case "Omni Directional":
            scene.omniDirLS[2] = value
            break
          case "Lower Ambient":
            scene.lowerAmbientLS[2] = value
            break
          case "Upper Ambient":
            scene.upperAmbientLS[2] = value
            break
        }
      }
    },

    wirefrmToggled(state) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        scene.drawWirefrm = !scene.drawWirefrm
      }
    },

    normalsToggled(state) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        scene.drawNormals = !scene.drawNormals
      }
    },

    materialFilterChanged(state, { payload: filter }: PayloadAction<string>) {
      const scene = state.allScenes[state.currIndex]
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
  rChanged,
  gChanged,
  bChanged,
  wirefrmToggled,
  normalsToggled,
  materialFilterChanged,
} = sceneSlice.actions

export function selectCurrScene(state: RootState) {
  return state.allScenes[state.currIndex]
}

export function selectCameraIdx(state: RootState) {
  return state.allScenes[state.currIndex]?.cameraIdx
}

export function selectFieldOfView(state: RootState) {
  const scene = state.allScenes[state.currIndex]
  return scene?.cameras[scene.cameraIdx].fieldOfView
}

export function selectAspectRatio(state: RootState) {
  const scene = state.allScenes[state.currIndex]
  return scene?.cameras[scene.cameraIdx].aspectRatio
}

export function selectLightSource(state: RootState) {
  return state.allScenes[state.currIndex]?.currentLS
}

export function selectLightSourceR(state: RootState) {
  const currScene = state.allScenes[state.currIndex]
  if (currScene) {
    switch (currScene.currentLS) {
      case "Omni Directional":
        return currScene.omniDirLS[0]
      case "Lower Ambient":
        return currScene.lowerAmbientLS[0]
      case "Upper Ambient":
        return currScene.upperAmbientLS[0]
    }
  }

  return undefined
}

export function selectLightSourceG(state: RootState) {
  const currScene = state.allScenes[state.currIndex]
  if (currScene) {
    switch (currScene.currentLS) {
      case "Omni Directional":
        return currScene.omniDirLS[1]
      case "Lower Ambient":
        return currScene.lowerAmbientLS[1]
      case "Upper Ambient":
        return currScene.upperAmbientLS[1]
    }
  }
  return undefined
}

export function selectLightSourceB(state: RootState) {
  const currScene = state.allScenes[state.currIndex]
  if (currScene) {
    switch (currScene.currentLS) {
      case "Omni Directional":
        return currScene.omniDirLS[2]
      case "Lower Ambient":
        return currScene.lowerAmbientLS[2]
      case "Upper Ambient":
        return currScene.upperAmbientLS[2]
    }
  }
  return undefined
}

export function selectDrawWirefrm(state: RootState) {
  return state.allScenes[state.currIndex]?.drawWirefrm
}

export function selectDrawNormals(state: RootState) {
  return state.allScenes[state.currIndex]?.drawNormals
}

export function selectMaterialFilter(state: RootState) {
  return state.allScenes[state.currIndex]?.materialFilter
}

export function selectMaterialsSelector(state: RootState) {
  return state.allScenes[state.currIndex]?.filteredMaterials
}
