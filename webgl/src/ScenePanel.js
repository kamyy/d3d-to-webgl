import React from 'react'
import PropTypes from 'prop-types'
import { connect } from 'react-redux';

import './App.css'
import actionCreators from './Actions';

function ScenePanel(props) {
    const { sceneArray, curSceneId, onClick } = props;

    return <div id='ScenePanel' className='CanvasPanel'> {
        sceneArray.map((scene, id) => {
            if (curSceneId === id) {
                return <button 
                    className='SceneButton' key={id} onClick={() => onClick(id)} style={{color: 'cyan', fontWeight: 'bold'}}>
                    {scene.name}
                </button>
            } else {
                return <button 
                    className='SceneButton' key={id} onClick={() => onClick(id)}>
                    {scene.name}
                </button>
            }
        })
    } </div>
}

ScenePanel.propTypes = {
    sceneArray: PropTypes.array.isRequired,
    curSceneId: PropTypes.number.isRequired,
    onClick: PropTypes.func.isRequired
}

const ConnectedScenePanel = connect(
    function({ sceneArray, curSceneId }) { 
        return { 
            sceneArray, 
            curSceneId 
        };
    },
    function(dispatch) { 
        return { 
            onClick: id => dispatch(actionCreators.changeCurScene(id)) 
        };
    }
)
(ScenePanel);

export default ConnectedScenePanel;
