import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import './App.css';
import { sceneArray } from './App.js';
import { actionCreators } from './Actions.js';

function RenderPanel(props) {
    const {
        sceneState,
        curSceneId,
        onToggleWirefrm,
        onToggleNormals,
    } = props;

    if (sceneState) {
        sceneArray.curScene.requestDrawScene();

        const id0 = 'wirefrmCheckbox';
        const id1 = 'normalsCheckbox';

        return <div>
            <span id='wirefrm-choice' className='CanvasPanel'>
                <input id={id0} type='checkbox' checked={sceneState.drawWirefrm} onChange={() => onToggleWirefrm(curSceneId)} className='Checkbox' />
                <label htmlFor={id0} className='CheckboxLabel'>Primitive Edges Only</label>
            </span>
            <span id='normals-choice' className='CanvasPanel'>
                <input id={id1} type='checkbox' checked={sceneState.drawNormals} onChange={() => onToggleNormals(curSceneId)} className='Checkbox' />
                <label htmlFor={id1} className='CheckboxLabel'>Vertex Normals</label>
            </span>
        </div>
    }

    return null;
}

RenderPanel.propTypes = {
    sceneState: PropTypes.object,
    curSceneId: PropTypes.number.isRequired,
    onToggleWirefrm: PropTypes.func.isRequired,
    onToggleNormals: PropTypes.func.isRequired,
};

const ConnectedRenderPanel = connect(
    function({ sceneArray, curSceneId }) { 
        return { 
            sceneState: sceneArray[curSceneId], curSceneId 
        };
    },
    function(dispatch) { 
        return { 
            onToggleWirefrm: (id) => dispatch(actionCreators.toggleWirefrm(id)),
            onToggleNormals: (id) => dispatch(actionCreators.toggleNormals(id)),
        };
    }
)(RenderPanel);

export default ConnectedRenderPanel;
