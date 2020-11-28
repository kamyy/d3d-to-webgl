import React from 'react'
import { useSelector, useDispatch } from 'react-redux'

import '../App.css'
import { rawScenes } from '../App.js'
import {
  lightSourceChanged,
  rChanged,
  gChanged,
  bChanged,
  selectCurrScene,
  selectLightSource,
  selectLightSourceR,
  selectLightSourceG,
  selectLightSourceB,
} from '../store/sceneSlice.js'

export default function LightsPanel() {
  const dispatch = useDispatch()
  const curScene = useSelector(selectCurrScene)

  const currentLS = useSelector(selectLightSource)
  const r = useSelector(selectLightSourceR)
  const g = useSelector(selectLightSourceG)
  const b = useSelector(selectLightSourceB)

  if (curScene) {
    rawScenes[curScene.id].requestDrawScene()

    return (
      <div id="LightPanel" className="CanvasPanel">
        <fieldset className="Fieldset">
          <legend className="Legend">Light Source</legend>
          <select
            className="LightSelect"
            value={currentLS}
            onChange={(event) => dispatch(lightSourceChanged(event.target.value))}
          >
            <option>Omni Directional</option>
            <option>Lower Ambient</option>
            <option>Upper Ambient</option>
          </select>
        </fieldset>

        <fieldset style={{ borderColor: 'red' }} className="Fieldset">
          <legend style={{ color: 'red' }} className="Legend">
            R <span>{(r * 255.0).toFixed(0)}</span>
          </legend>
          <input
            className="Range"
            type="range"
            min="0"
            max="255"
            value={r * 255.0}
            onChange={(event) => dispatch(rChanged(event.target.value / 255.0))}
          />
        </fieldset>

        <fieldset style={{ borderColor: 'green' }} className="Fieldset">
          <legend style={{ color: 'green' }} className="Legend">
            G <span>{(g * 255.0).toFixed(0)}</span>
          </legend>
          <input
            className="Range"
            type="range"
            min="0"
            max="255"
            value={g * 255.0}
            onChange={(event) => dispatch(gChanged(event.target.value / 255.0))}
          />
        </fieldset>

        <fieldset style={{ borderColor: 'blue' }} className="Fieldset">
          <legend style={{ color: 'blue' }} className="Legend">
            B <span>{(b * 255.0).toFixed(0)}</span>
          </legend>
          <input
            className="Range"
            type="range"
            min="0"
            max="255"
            value={b * 255.0}
            onChange={(event) => dispatch(bChanged(event.target.value / 255.0))}
          />
        </fieldset>
      </div>
    )
  }
  return null
}
