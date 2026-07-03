import { configureStore } from "@reduxjs/toolkit"
import sceneReducer, { selectCurrScene } from "./sceneSlice"

const store = configureStore({
  reducer: sceneReducer,
  middleware: (getDefaultMiddleware) => getDefaultMiddleware({ serializableCheck: false }),
})

export const getCurrScene = () => {
  return selectCurrScene(store.getState())
}

export type RootState = ReturnType<typeof store.getState>
export type AppDispatch = typeof store.dispatch

export default store
