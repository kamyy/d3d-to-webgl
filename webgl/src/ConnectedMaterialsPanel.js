import React from 'react'
import PropTypes from 'prop-types'
import { connect } from 'react-redux'

import './App.css'
import { sceneArray } from './App.js'
import { actionCreators } from './Actions.js'

function MaterialsPanel(props) {
  const { sceneState, curSceneId, materialFilter, setOfMaterials, onChangeMaterialFilter } = props

  if (sceneState) {
    sceneArray.curScene.requestDrawScene()

    return (
      <div id="MaterialPanel" className="CanvasPanel">
        <fieldset className="Fieldset">
          <legend className="Legend">Filter By Material</legend>
          <input
            type="text"
            value={materialFilter}
            spellCheck={false}
            onChange={(event) => onChangeMaterialFilter(curSceneId, event.target.value)}
            className="MaterialInput"
          />
          <span id="Clear" onClick={() => onChangeMaterialFilter(curSceneId, '')}>
            &times;
          </span>
        </fieldset>

        <fieldset className="Fieldset">
          <legend className="Legend">Materials</legend>{' '}
          {[...setOfMaterials].map((m) => (
            <div key={m.name} className="MaterialItem" onClick={() => onChangeMaterialFilter(curSceneId, m.name)}>
              {m.name}
            </div>
          ))}{' '}
        </fieldset>
      </div>
    )
  }

  return null
}

MaterialsPanel.propTypes = {
  sceneState: PropTypes.object,
  curSceneId: PropTypes.number.isRequired,
  materialFilter: PropTypes.string,
  setOfMaterials: PropTypes.object,
  onChangeMaterialFilter: PropTypes.func.isRequired,
}

const ConnectedMaterialsPanel = connect(
  function ({ sceneArray, curSceneId }) {
    const sceneState = sceneArray[curSceneId]
    return {
      sceneState,
      curSceneId,
      materialFilter: sceneState ? sceneState.materialFilter : null,
      setOfMaterials: sceneState ? sceneState.setOfMaterials : null,
    }
  },
  function (dispatch) {
    return {
      onChangeMaterialFilter: (id, filter) => dispatch(actionCreators.changeMaterialFilter(id, filter)),
    }
  }
)(MaterialsPanel)

export default ConnectedMaterialsPanel
