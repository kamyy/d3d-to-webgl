import React from 'react'
import { useSelector, useDispatch } from 'react-redux'

import '../App.css'
import { rawScenes } from '../App.js'
import {
  cameraChanged,
  fieldOfViewChanged,
  aspectRatioChanged,
  selectCurrScene,
  selectCameraIdx,
  selectFieldOfView,
  selectAspectRatio,
} from '../store/sceneSlice.js'

export default function CameraPanel() {
  const dispatch = useDispatch()
  const curScene = useSelector(selectCurrScene)
  const cameraIdx = useSelector(selectCameraIdx)
  const fieldOfView = useSelector(selectFieldOfView)
  const aspectRatio = useSelector(selectAspectRatio)

  if (curScene) {
    rawScenes[curScene.id].requestDrawScene()

    const cameraChoice = (cam, idx) => {
      const id = `Camera ${idx + 1}`
      return (
        <div key={id}>
          <input
            id={id}
            type="radio"
            className="CameraButton"
            checked={cameraIdx === idx}
            onChange={() => dispatch(cameraChanged(idx))}
          />
          <label htmlFor={id} className="CameraButtonLabel">
            {id}{' '}
          </label>
        </div>
      )
    }

    return (
      <div id="CameraPanel" className="CanvasPanel">
        {curScene.cameras.length > 1 ? (
          <div style={{ marginBottom: '8px' }}>{curScene.cameras.map(cameraChoice)}</div>
        ) : (
          <div style={{ marginBottom: '4px' }} />
        )}

        <fieldset className="Fieldset">
          <legend className="Legend">
            Field of View <span className="LegendValue">{fieldOfView.toFixed(0)} </span>
          </legend>
          <input
            className="Range"
            type="range"
            min="15"
            max="165"
            value={fieldOfView.toFixed(0)}
            onChange={(event) => dispatch(fieldOfViewChanged(parseInt(event.target.value, 10)))}
          />
        </fieldset>

        <fieldset className="Fieldset">
          <legend className="Legend">
            Aspect Ratio <span className="LegendValue">{aspectRatio.toFixed(2)} </span>
          </legend>
          <input
            className="Range"
            type="range"
            min="50"
            max="500"
            value={aspectRatio * 100}
            onChange={(event) => dispatch(aspectRatioChanged(parseInt(event.target.value, 10) * 0.01))}
          />
        </fieldset>
      </div>
    )
  }
  return null
}
