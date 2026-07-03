import { StrictMode } from "react"
import { createRoot } from "react-dom/client"
import { Provider } from "react-redux"

import store from "../redux/store"
import App from "./App"
import "./index.css"

const rootElement = document.getElementById("root")
if (!rootElement) {
  throw new Error("Root element not found")
}

createRoot(rootElement).render(
  <StrictMode>
    <Provider store={store}>
      <App />
    </Provider>
  </StrictMode>
)
