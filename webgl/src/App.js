import React, { useState, useEffect } from 'react'
import { useSelector } from 'react-redux'

import { selectCurrScene } from './store/sceneSlice.js'
import { getCurrScene } from './store/appStore.js'
import './App.css'

import Scene from './Scene.js'
import Vector1x4 from './Vector1x4.js'
import ShaderP3C3 from './shader/ShaderP3C3.js'
import ShaderP3N3 from './shader/ShaderP3N3.js'
import ShaderP3N3T2 from './shader/ShaderP3N3T2.js'
import ShaderP3N3B3T2 from './shader/ShaderP3N3B3T2.js'

import ScenePanel from './panel/ScenePanel.js'
import CameraPanel from './panel/CameraPanel.js'
import RenderPanel from './panel/RenderPanel.js'
import LightsPanel from './panel/LightsPanel.js'
import MaterialsPanel from './panel/MaterialsPanel.js'

export let rawScenes = [new Scene(0, 'hardwood'), new Scene(1, 'biplane'), new Scene(2, 'goku')]
export let GL = null

let lButtonDown = false
let rButtonDown = false
let lx = 0
let ly = 0

function degreesToRadians(degrees) {
  return (degrees * Math.PI) / 180.0
}

function onMouseDown(event) {
  const canvas = document.getElementById('Canvas')
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

function onMouseUp(event) {
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

function onMouseMove(event) {
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
          // dolly
          camera.translate(new Vector1x4(0, (x - lx) * TXYZ_SCALAR, 0))
          lx = x
          ly = y
          scene.requestDrawScene()
        } else if ((lButtonDown && event.ctrlKey) || rButtonDown) {
          // move
          const dx = (lx - x) * TXYZ_SCALAR
          const dz = (y - ly) * TXYZ_SCALAR
          const dv = camera.mapPos(new Vector1x4(dx, 0, dz, 0), target)
          target.translate(dv) // move target along own axes
          lx = x
          ly = y
          scene.requestDrawScene()
        } else if (lButtonDown) {
          // rotate
          target.rotateZ(degreesToRadians(lx - x) * RXYZ_SCALAR) // yaw camera target around it's own z-axis
          camera.rotateX(degreesToRadians(ly - y) * RXYZ_SCALAR, target) // pitch around camera target's x-axis
          lx = x
          ly = y
          scene.requestDrawScene()
        }
      }
    }
  }
}

export default function App() {
  const [initial, setInitial] = useState(true)

  useEffect(() => {
    if (initial) {
      const canvas = document.getElementById('Canvas')
      GL = canvas.getContext('experimental-webgl', {
        depth: true,
        alpha: false,
        stencil: true,
      })

      if (GL) {
        canvas.oncontextmenu = (event) => event.preventDefault() // disable right click context menu
        canvas.onmousedown = onMouseDown
        window.onmousemove = onMouseMove
        window.onmouseup = onMouseUp

        GL.depthFunc(GL.LESS) // less than depth test
        GL.enable(GL.DEPTH_TEST) // enable depth testing
        GL.enable(GL.CULL_FACE) // enable backface culling
        GL.enable(GL.BLEND) // enable alpha blending
        GL.blendFunc(GL.SRC_ALPHA, GL.ONE_MINUS_SRC_ALPHA)

        GL.clearColor(0.392156899, 0.58431375, 0.929411829, 1.0) // cornflower blue
        GL.clearDepth(1.0) // depth buffer clear value
        GL.clearStencil(0) // stencil buffer clear value

        GL.mapOfShaders = Object.freeze(
          new Map([
            ['P3C3', new ShaderP3C3()],
            ['P3N3', new ShaderP3N3()],
            ['P3N3T2', new ShaderP3N3T2()],
            ['P3N3B3T2', new ShaderP3N3B3T2()],
          ])
        )

        rawScenes[0].loadScene()
        setInitial(false)
      }
    }
  })

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
