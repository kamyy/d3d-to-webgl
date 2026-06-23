import { configureStore } from "@reduxjs/toolkit"
import rootReducer, { selectCurrScene } from "./sceneSlice"

const appStore = configureStore({
  reducer: rootReducer,
  middleware: (getDefaultMiddleware) => getDefaultMiddleware({ serializableCheck: false }),
})

export const getCurrScene = () => {
  return selectCurrScene(appStore.getState())
}

export default appStore
