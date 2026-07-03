import "../app/App.css"
import { rawScenes } from "../app/App"
import {
  wireframeToggled,
  normalsToggled,
  selectCurrScene,
  selectDrawWireframe,
  selectDrawNormals,
} from "../redux/sceneSlice"
import { useAppDispatch, useAppSelector } from "../redux/hooks"

export default function RenderPanel() {
  const dispatch = useAppDispatch()
  const curScene = useAppSelector(selectCurrScene)
  const drawWireframe = useAppSelector(selectDrawWireframe)
  const drawNormals = useAppSelector(selectDrawNormals)

  if (curScene && drawWireframe !== undefined && drawNormals !== undefined) {
    const id0 = "wireframeCheckbox"
    const id1 = "normalsCheckbox"

    rawScenes[curScene.id].requestDrawScene()

    return (
      <div>
        <span id="wirefrm-choice" className="CanvasPanel">
          <input
            id={id0}
            type="checkbox"
            checked={drawWireframe}
            onChange={() => dispatch(wireframeToggled())}
            className="Checkbox"
          />
          <label htmlFor={id0} className="CheckboxLabel">
            Primitive Edges Only
          </label>
        </span>
        <span id="normals-choice" className="CanvasPanel">
          <input
            id={id1}
            type="checkbox"
            checked={drawNormals}
            onChange={() => dispatch(normalsToggled())}
            className="Checkbox"
          />
          <label htmlFor={id1} className="CheckboxLabel">
            Vertex Normals
          </label>
        </span>
      </div>
    )
  }

  return null
}
