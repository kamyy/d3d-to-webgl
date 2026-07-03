import "../app/App.css"
import { rawScenes } from "../app/App"
import {
  materialFilterChanged,
  selectCurrScene,
  selectMaterialFilter,
  selectFilteredMaterials,
} from "../redux/sceneSlice"
import { useAppDispatch, useAppSelector } from "../redux/hooks"

export default function MaterialsPanel() {
  const dispatch = useAppDispatch()
  const curScene = useAppSelector(selectCurrScene)
  const materialFilter = useAppSelector(selectMaterialFilter)
  const setOfMaterials = useAppSelector(selectFilteredMaterials)

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
