import React from 'react'
import PropTypes from 'prop-types'
import { connect } from 'react-redux';

import './App.css'
import { actionCreators } from './Actions';

function MaterialsPanel(props) {
    const {
        sceneState,
        curSceneId,
        materialFilter,
        setOfMaterials,
        onChangeMaterialFilter,
    } = props;

    if (sceneState) {
        return <div id='MaterialPanel' className='CanvasPanel'>
            <fieldset className='Fieldset'><legend className='Legend'>Filter By Material</legend>
                <input  type='text' 
                        value={materialFilter} 
                        spellCheck={false} 
                        onChange={event => onChangeMaterialFilter(curSceneId, event.target.value)}
                        className='MaterialInput' 
                        />
                <span onClick={event => onChangeMaterialFilter(curSceneId, '')}>&times;</span>
            </fieldset>

            <fieldset className='Fieldset'><legend className='Legend'>Materials</legend> {
                [...setOfMaterials].map(m => 
                    <div key={m.name} className='MaterialItem' onClick={event => onChangeMaterialFilter(curSceneId, event.key)}>{m.name}</div>
                )
            } </fieldset>
        </div>
    }

    return null;
}

MaterialsPanel.propTypes = {
    sceneState: PropTypes.object.isRequired,
    curSceneId: PropTypes.number.isRequired,
    materialFilter: PropTypes.string.isRequired,
    setOfMaterials: PropTypes.set.isRequired,
    onChangeMaterialFilter: PropTypes.func.isRequired,
};

const ConnectedMaterialsPanel = connect(
    function({ sceneArray, curSceneId, materialFilter, setOfMaterials }) {
        return { 
            sceneState: sceneArray[curSceneId], curSceneId, materialFilter, setOfMaterials
        };
    },
    function(dispatch) {
        return {
            onChangeMaterialFilter: (id, filter) => dispatch(actionCreators.changeMaterialFilter(id, filter))
        };
    }
)(MaterialsPanel);

export default ConnectedMaterialsPanel;
