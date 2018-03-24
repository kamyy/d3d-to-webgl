import React from 'react'
import PropTypes from 'prop-types'
import { connect } from 'react-redux';

import './App.css'
import { sceneArray } from './App';
import { actionCreators } from './Actions';

function ScenePanel(props) {
    const { curSceneId, onChangeCurScene } = props;

    return <div id='ScenePanel' className='CanvasPanel'> {
        sceneArray.map((scene, id) => {
            if (curSceneId === id) {
                return <button className='SceneButton' key={id} onClick={() => onChangeCurScene(id)} style={{color: 'cyan', fontWeight: 'bold'}}> {scene.name} </button>
            } else {
                return <button className='SceneButton' key={id} onClick={() => onChangeCurScene(id)}> {scene.name} </button>
            }
        })
    } </div>
}

ScenePanel.propTypes = {
    curSceneId: PropTypes.number.isRequired,
    onChangeCurScene: PropTypes.func.isRequired,
}

const ConnectedScenePanel = connect(
    function({ curSceneId }) { 
        return { curSceneId };
    },
    function(dispatch) { 
        return { onChangeCurScene: id => dispatch(actionCreators.changeCurScene(id)) };
    }
)(ScenePanel);

export default ConnectedScenePanel;
