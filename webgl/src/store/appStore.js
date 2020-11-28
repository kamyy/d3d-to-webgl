import { configureStore, getDefaultMiddleware } from '@reduxjs/toolkit'
import rootReducer, { selectCurrScene } from './sceneSlice.js'

const appStore = configureStore({
  middleware: getDefaultMiddleware({ serializableCheck: false }),
  reducer: rootReducer,
})

export const getCurrScene = function () {
  return selectCurrScene(appStore.getState())
}

export default appStore
