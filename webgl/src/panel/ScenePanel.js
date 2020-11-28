import React, { useState } from 'react'
import { useDispatch } from 'react-redux'

import '../App.css'
import { rawScenes } from '../App.js'
import { sceneChanged } from '../store/sceneSlice.js'

export default function ScenePanel() {
  const dispatch = useDispatch()
  const [currSceneId, setCurrSceneId] = useState(0)

  rawScenes[currSceneId].loadScene()
  rawScenes[currSceneId].requestDrawScene()

  return (
    <div id="ScenePanel" className="CanvasPanel">
      {' '}
      {rawScenes.map((scene, i) =>
        i === currSceneId ? (
          <button className="SceneButton" key={i} style={{ color: 'cyan', fontWeight: 'bold' }}>
            {' '}
            {scene.name}{' '}
          </button>
        ) : (
          <button
            className="SceneButton"
            key={i}
            onClick={() => {
              setCurrSceneId(i)
              dispatch(sceneChanged(i))
            }}
          >
            {' '}
            {scene.name}{' '}
          </button>
        )
      )}{' '}
    </div>
  )
}
