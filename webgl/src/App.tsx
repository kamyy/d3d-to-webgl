import { useEffect } from "react"
import { useSelector } from "react-redux"

import { selectCurrScene } from "./store/sceneSlice"
import { getCurrScene } from "./store/appStore"
import "./App.css"

import Scene from "./Scene"
import Vector1x4 from "./Vector1x4"
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

let lButtonDown = false
let rButtonDown = false
let lx = 0
let ly = 0

function degreesToRadians(degrees: number) {
  return (degrees * Math.PI) / 180.0
}

function onMouseDown(event: MouseEvent) {
  const canvas = document.getElementById("Canvas") as HTMLCanvasElement | null
  if (!canvas) {
    return
  }
  const rect = canvas.getBoundingClientRect()
  const x = event.clientX
  const y = event.clientY

  if (x > rect.left && x < rect.right && y > rect.top && y < rect.bottom) {
    switch (event.button) {
      case 0:
        lButtonDown = true
        break
      case 2:
        rButtonDown = true
        break
      default:
        break
    }
    lx = x
    ly = y
  }
}

function onMouseUp(event: MouseEvent) {
  switch (event.button) {
    case 0:
      lButtonDown = false
      break
    case 2:
      rButtonDown = false
      break
    default:
      break
  }
}

function onMouseMove(event: MouseEvent) {
  if (lButtonDown || rButtonDown) {
    const currScene = getCurrScene()

    if (currScene) {
      const scene = rawScenes[currScene.id]
      const camera = scene.activeCamera

      if (camera) {
        const TXYZ_SCALAR = 0.01
        const RXYZ_SCALAR = 0.25
        const x = event.clientX
        const y = event.clientY
        const target = camera.parent

        if ((lButtonDown && rButtonDown) || (lButtonDown && event.shiftKey)) {
          camera.translate(new Vector1x4(0, (x - lx) * TXYZ_SCALAR, 0))
          lx = x
          ly = y
          scene.requestDrawScene()
        } else if ((lButtonDown && event.ctrlKey) || rButtonDown) {
          const dx = (lx - x) * TXYZ_SCALAR
          const dz = (y - ly) * TXYZ_SCALAR
          const dv = camera.mapPos(new Vector1x4(dx, 0, dz, 0), target ?? undefined)
          if (target) {
            target.translate(dv)
          }
          lx = x
          ly = y
          scene.requestDrawScene()
        } else if (lButtonDown) {
          target?.rotateZ(degreesToRadians(lx - x) * RXYZ_SCALAR)
          camera.rotateX(degreesToRadians(ly - y) * RXYZ_SCALAR, target ?? undefined)
          lx = x
          ly = y
          scene.requestDrawScene()
        }
      }
    }
  }
}

export default function App() {
  useEffect(() => {
    const canvas = document.getElementById("Canvas") as HTMLCanvasElement | null
    if (!canvas) {
      return
    }

    const context = canvas.getContext("experimental-webgl", {
      depth: true,
      alpha: false,
      stencil: true,
    })

    if (context) {
      GL = context as AppWebGLContext
      canvas.oncontextmenu = (event) => event.preventDefault()
      canvas.onmousedown = onMouseDown
      window.onmousemove = onMouseMove
      window.onmouseup = onMouseUp

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
        <span className="MouseInfoBold">* Translate</span> Right click + drag. Or ctrl + left click + drag.
      </p>
      <p className="MouseInfo">
        <span className="MouseInfoBold">* Dolly In/Out</span> Left + right click + drag. Or shift + left click + drag.
      </p>
    </div>
  )
}
