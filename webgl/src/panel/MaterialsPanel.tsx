import { useSelector, useDispatch } from "react-redux"

import "../app/App.css"
import { rawScenes } from "../app/App"
import {
  materialFilterChanged,
  selectCurrScene,
  selectMaterialFilter,
  selectMaterialsSelector,
} from "../store/sceneSlice"

export default function MaterialsPanel() {
  const dispatch = useDispatch()
  const curScene = useSelector(selectCurrScene)
  const materialFilter = useSelector(selectMaterialFilter)
  const setOfMaterials = useSelector(selectMaterialsSelector)

  if (curScene && materialFilter !== undefined && setOfMaterials) {
    rawScenes[curScene.id].requestDrawScene()

    return (
      <div id="MaterialPanel" className="CanvasPanel">
        <fieldset className="Fieldset">
          <legend className="Legend">Filter By Material</legend>
          <input
            type="text"
            value={materialFilter}
            spellCheck={false}
            onChange={(event) => dispatch(materialFilterChanged(event.target.value))}
            className="MaterialInput"
          />
          <span id="Clear" onClick={() => dispatch(materialFilterChanged(""))}>
            &times;
          </span>
        </fieldset>

        <fieldset className="Fieldset">
          <legend className="Legend">Materials</legend>{" "}
          {[...setOfMaterials].map((m) => (
            <div key={m.name} className="MaterialItem" onClick={() => dispatch(materialFilterChanged(m.name))}>
              {m.name}
            </div>
          ))}{" "}
        </fieldset>
      </div>
    )
  }

  return null
}
