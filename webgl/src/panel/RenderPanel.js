import React from 'react'
import { useSelector, useDispatch } from 'react-redux'

import '../App.css'
import { rawScenes } from '../App.js'
import {
  wirefrmToggled,
  normalsToggled,
  selectCurrScene,
  selectDrawWirefrm,
  selectDrawNormals,
} from '../store/sceneSlice.js'

export default function RenderPanel() {
  const dispatch = useDispatch()
  const curScene = useSelector(selectCurrScene)
  const drawWirefrm = useSelector(selectDrawWirefrm)
  const drawNormals = useSelector(selectDrawNormals)

  if (curScene) {
    const id0 = 'wirefrmCheckbox'
    const id1 = 'normalsCheckbox'

    rawScenes[curScene.id].requestDrawScene()

    return (
      <div>
        <span id="wirefrm-choice" className="CanvasPanel">
          <input
            id={id0}
            type="checkbox"
            checked={drawWirefrm}
            onChange={() => dispatch(wirefrmToggled())}
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
