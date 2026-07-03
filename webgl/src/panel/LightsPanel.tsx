import "../app/App.css"
import { rawScenes } from "../app/App"
import {
  lightSourceChanged,
  lightColorChannelChanged,
  selectCurrScene,
  selectLightSource,
  selectLightSourceRed,
  selectLightSourceGreen,
  selectLightSourceBlue,
  type LightSourceType,
} from "../redux/sceneSlice"
import { useAppDispatch, useAppSelector } from "../redux/hooks"

export default function LightsPanel() {
  const dispatch = useAppDispatch()
  const curScene = useAppSelector(selectCurrScene)

  const currentLS = useAppSelector(selectLightSource)
  const r = useAppSelector(selectLightSourceRed)
  const g = useAppSelector(selectLightSourceGreen)
  const b = useAppSelector(selectLightSourceBlue)

  if (curScene && currentLS && r !== undefined && g !== undefined && b !== undefined) {
    rawScenes[curScene.id].requestDrawScene()

    return (
      <div id="LightPanel" className="CanvasPanel">
        <fieldset className="Fieldset">
          <legend className="Legend">Light Source</legend>
          <select
            className="LightSelect"
            value={currentLS}
            onChange={(event) => dispatch(lightSourceChanged(event.target.value as LightSourceType))}
          >
            <option>Omni Directional</option>
            <option>Lower Ambient</option>
            <option>Upper Ambient</option>
          </select>
        </fieldset>

        <fieldset style={{ borderColor: "red" }} className="Fieldset">
          <legend style={{ color: "red" }} className="Legend">
            R <span>{(r * 255.0).toFixed(0)}</span>
          </legend>
          <input
            className="Range"
            type="range"
            min="0"
            max="255"
            value={r * 255.0}
            onChange={(event) =>
              dispatch(lightColorChannelChanged({ channel: 0, value: parseFloat(event.target.value) / 255.0 }))
            }
          />
        </fieldset>

        <fieldset style={{ borderColor: "green" }} className="Fieldset">
          <legend style={{ color: "green" }} className="Legend">
            G <span>{(g * 255.0).toFixed(0)}</span>
          </legend>
          <input
            className="Range"
            type="range"
            min="0"
            max="255"
            value={g * 255.0}
            onChange={(event) =>
              dispatch(lightColorChannelChanged({ channel: 1, value: parseFloat(event.target.value) / 255.0 }))
            }
          />
        </fieldset>

        <fieldset style={{ borderColor: "blue" }} className="Fieldset">
          <legend style={{ color: "blue" }} className="Legend">
            B <span>{(b * 255.0).toFixed(0)}</span>
          </legend>
          <input
            className="Range"
            type="range"
            min="0"
            max="255"
            value={b * 255.0}
            onChange={(event) =>
              dispatch(lightColorChannelChanged({ channel: 2, value: parseFloat(event.target.value) / 255.0 }))
            }
          />
        </fieldset>
      </div>
    )
  }

  return null
}
