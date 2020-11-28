import { createSlice } from '@reduxjs/toolkit'

const sceneSlice = createSlice({
  name: 'scene',

  initialState: {
    allScenes: [null, null, null],
    currIndex: 0,
  },

  reducers: {
    sceneLoaded(state, { payload: { id, scene } }) {
      const { cameras, omniDirLS, mapOfMaterials } = scene
      state.allScenes[id] = {
        id,
        cameras: cameras.map((cam) => ({ fieldOfView: cam.fieldOfView, aspectRatio: cam.aspectRatio })),
        cameraIdx: 0,
        currentLS: 'Omni Directional',
        omniDirLS: [...omniDirLS.color],
        lowerAmbientLS: [0.2, 0.2, 0.2],
        upperAmbientLS: [0.2, 0.2, 0.2],
        drawWirefrm: false,
        drawNormals: false,
        materialFilter: '',
        originalMaterials: Array.from(mapOfMaterials.values()),
        filteredMaterials: Array.from(mapOfMaterials.values()),
      }
    },

    sceneChanged(state, { payload: id }) {
      state.currIndex = id
    },

    cameraChanged(state, { payload: cameraIdx }) {
      state.allScenes[state.currIndex].cameraIdx = cameraIdx
    },

    fieldOfViewChanged(state, { payload: fieldOfView }) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        scene.cameras[scene.cameraIdx].fieldOfView = fieldOfView
      }
    },

    aspectRatioChanged(state, { payload: aspectRatio }) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        scene.cameras[scene.cameraIdx].aspectRatio = aspectRatio
      }
    },

    lightSourceChanged(state, { payload: lightSource }) {
      if (state.allScenes[state.currIndex]) {
        state.allScenes[state.currIndex].currentLS = lightSource
      }
    },

    rChanged(state, { payload: value }) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        switch (scene.currentLS) {
          case 'Omni Directional':
            scene.omniDirLS[0] = value
            break
          case 'Lower Ambient':
            scene.lowerAmbientLS[0] = value
            break
          case 'Upper Ambient':
            scene.upperAmbientLS[0] = value
            break
        }
      }
    },

    gChanged(state, { payload: value }) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        switch (scene.currentLS) {
          case 'Omni Directional':
            scene.omniDirLS[1] = value
            break
          case 'Lower Ambient':
            scene.lowerAmbientLS[1] = value
            break
          case 'Upper Ambient':
            scene.upperAmbientLS[1] = value
            break
        }
      }
    },

    bChanged(state, { payload: value }) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        switch (scene.currentLS) {
          case 'Omni Directional':
            scene.omniDirLS[2] = value
            break
          case 'Lower Ambient':
            scene.lowerAmbientLS[2] = value
            break
          case 'Upper Ambient':
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

    materialFilterChanged(state, { payload: filter }) {
      const scene = state.allScenes[state.currIndex]
      if (scene) {
        const lcFilter = filter.toLowerCase()
        const filterFn = (material) => material.name.toLowerCase().includes(lcFilter)

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

export const selectCurrScene = (state) => state.allScenes[state.currIndex]

export const selectCameraIdx = (state) => state.allScenes[state.currIndex]?.cameraIdx

export const selectFieldOfView = (state) => {
  const scene = state.allScenes[state.currIndex]
  return scene?.cameras[scene.cameraIdx].fieldOfView
}

export const selectAspectRatio = (state) => {
  const scene = state.allScenes[state.currIndex]
  return scene?.cameras[scene.cameraIdx].aspectRatio
}

export const selectLightSource = (state) => state.allScenes[state.currIndex]?.currentLS

export const selectLightSourceR = (state) => {
  const currScene = state.allScenes[state.currIndex]
  if (currScene) {
    switch (currScene.currentLS) {
      case 'Omni Directional':
        return currScene.omniDirLS[0]
      case 'Lower Ambient':
        return currScene.lowerAmbientLS[0]
      case 'Upper Ambient':
        return currScene.upperAmbientLS[0]
    }
  }
  return undefined
}

export const selectLightSourceG = (state) => {
  const currScene = state.allScenes[state.currIndex]
  if (currScene) {
    switch (currScene.currentLS) {
      case 'Omni Directional':
        return currScene.omniDirLS[1]
      case 'Lower Ambient':
        return currScene.lowerAmbientLS[1]
      case 'Upper Ambient':
        return currScene.upperAmbientLS[1]
    }
  }
  return undefined
}

export const selectLightSourceB = (state) => {
  const currScene = state.allScenes[state.currIndex]
  if (currScene) {
    switch (currScene.currentLS) {
      case 'Omni Directional':
        return currScene.omniDirLS[2]
      case 'Lower Ambient':
        return currScene.lowerAmbientLS[2]
      case 'Upper Ambient':
        return currScene.upperAmbientLS[2]
    }
  }
  return undefined
}

export const selectDrawWirefrm = (state) => state.allScenes[state.currIndex]?.drawWirefrm
export const selectDrawNormals = (state) => state.allScenes[state.currIndex]?.drawNormals

export const selectMaterialFilter = (state) => state.allScenes[state.currIndex]?.materialFilter
export const selectMaterialsSelector = (state) => state.allScenes[state.currIndex]?.filteredMaterials
