import { useEffect } from "react"
import { useSelector } from "react-redux"

import { selectCurrScene } from "./store/sceneSlice"
import { getCurrScene } from "./store/appStore"
import "./App.css"

import Scene from "./Scene"
import CameraControls from "./CameraControls"
import ShaderP3C3 from "./shader/ShaderP3C3"
import ShaderP3N3 from "./shader/ShaderP3N3"
import ShaderP3N3T2 from "./shader/ShaderP3N3T2"
import ShaderP3N3B3T2 from "./shader/ShaderP3N3B3T2"

import ScenePanel from "./panel/ScenePanel"
import CameraPanel from "./panel/CameraPanel"
import RenderPanel from "./panel/RenderPanel"
import LightsPanel from "./panel/LightsPanel"
import MaterialsPanel from "./panel/MaterialsPanel"
import { AppWebGLContext, ShaderProgramName } from "./types/webgl"
import Shader from "./shader/Shader"

export const rawScenes = [new Scene(0, "hardwood"), new Scene(1, "biplane"), new Scene(2, "goku")]
export let GL: AppWebGLContext | null = null

export default function App() {
  useEffect(() => {
    const canvas = document.getElementById("Canvas") as HTMLCanvasElement | null
    if (!canvas) {
      return
    }

    const context = canvas.getContext("webgl", {
      depth: true,
      alpha: false,
      stencil: true,
    })

    if (context) {
      GL = context as AppWebGLContext
      new CameraControls(canvas, () => {
        const currScene = getCurrScene()
        return currScene ? rawScenes[currScene.id] : null
      })

      GL.depthFunc(GL.LESS)
      GL.enable(GL.DEPTH_TEST)
      GL.enable(GL.CULL_FACE)
      GL.enable(GL.BLEND)
      GL.blendFunc(GL.SRC_ALPHA, GL.ONE_MINUS_SRC_ALPHA)

      GL.clearColor(0.392156899, 0.58431375, 0.929411829, 1.0)
      GL.clearDepth(1.0)
      GL.clearStencil(0)

      const shaders = new Map<ShaderProgramName, Shader>([
        ["P3C3", new ShaderP3C3()],
        ["P3N3", new ShaderP3N3()],
        ["P3N3T2", new ShaderP3N3T2()],
        ["P3N3B3T2", new ShaderP3N3B3T2()],
      ])
      GL.mapOfShaders = Object.freeze(shaders)

      rawScenes[0].loadScene()
    }
  }, [])

  const currScene = useSelector(selectCurrScene)

  return (
    <div className="App">
      <canvas id="Canvas" width="1280" height="720">
        Please use a browser that supports WebGL
      </canvas>

      {currScene === null ? <div className="Spinner" /> : null}

      <div id="LHS">
        <ScenePanel />
        <CameraPanel />
      </div>

      <div id="Bottom">
        <RenderPanel />
      </div>

      <div id="RHS">
        <LightsPanel />
        <MaterialsPanel />
      </div>

      <hr />

      <p className="ProjectInfo">MIT License</p>
      <p className="ProjectInfo">
        <a href="https://github.com/kamyy/d3d-to-webgl">Project @ GitHub</a>
      </p>
      <p className="ProjectInfo">
        Copyright &copy; 2018 <a href="mailto:kam.yin.yip@gmail.com">Kam Y Yip</a>
      </p>

      <br />

      <p className="MouseInfo">
        <span className="MouseInfoBold">* Rotate</span> Left click + drag.
      </p>
      <p className="MouseInfo">
        <span className="MouseInfoBold">* Translate</span> Right click + drag.
      </p>
      <p className="MouseInfo">
        <span className="MouseInfoBold">* Zoom</span> Mouse wheel. Or middle click + drag up/down.
      </p>
    </div>
  )
}
