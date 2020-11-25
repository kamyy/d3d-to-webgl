import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import './App.css';
import { sceneArray } from './App.js';
import { actionCreators } from './Actions.js';

function CameraPanel(props) {
    const { 
        sceneState, 
        curSceneId, 
        onChangeCamera, 
        onChangeFieldOfView, 
        onChangeAspectRatio,
    } = props;

    if (sceneState) {
        sceneArray.curScene.requestDrawScene();

        const cameraChoice = (cam, idx) => { 
            const id = `Camera ${idx + 1}`;
            return <div key={id}> 
                <input 
                    id={id}
                    type='radio'
                    className='CameraButton' 
                    checked={sceneState.cameraIdx === idx} 
                    onChange={() => onChangeCamera(curSceneId, idx)} />
                <label 
                    htmlFor={id} 
                    className='CameraButtonLabel'>
                    {id} </label>
            </div>
        }

        const cameraState = sceneState.cameras[sceneState.cameraIdx];

        return <div id='CameraPanel' className='CanvasPanel'> 
            {  
                (sceneState.cameras.length > 1) ?
                    <div style={{marginBottom: '8px'}}>{sceneState.cameras.map(cameraChoice)}</div>
                    :
                    <div style={{marginBottom: '4px'}}/>
            }

            <fieldset className='Fieldset'> 
                <legend className='Legend'>
                    Field of View <span className='LegendValue'>{cameraState.fieldOfView.toFixed(0)} </span>
                </legend>
                <input
                    className='Range'
                    type='range'
                    min='15'
                    max='165'
                    value={cameraState.fieldOfView.toFixed(0)} 
                    onChange={event => onChangeFieldOfView(curSceneId, parseInt(event.target.value, 10))} />
            </fieldset>

            <fieldset className='Fieldset'> 
                <legend className='Legend'>
                    Aspect Ratio <span className='LegendValue'>{cameraState.aspectRatio.toFixed(2)} </span>
                </legend>
                <input  
                    className='Range'
                    type='range' 
                    min='50' 
                    max='500'
                    value={cameraState.aspectRatio * 100}
                    onChange={event => onChangeAspectRatio(curSceneId, parseInt(event.target.value, 10) * 0.01)} />
            </fieldset>
        </div>
    }
    return null;
}

CameraPanel.propTypes = {
    sceneState: PropTypes.object,
    curSceneId: PropTypes.number.isRequired,
    onChangeCamera: PropTypes.func.isRequired,
    onChangeFieldOfView: PropTypes.func.isRequired,
    onChangeAspectRatio: PropTypes.func.isRequired,
}

const ConnectedCameraPanel = connect(
    function({ sceneArray, curSceneId }) { 
        return { 
            sceneState: sceneArray[curSceneId], curSceneId 
        };
    },
    function(dispatch) { 
        return { 
            onChangeCamera: (id, cameraIdx) => dispatch(actionCreators.changeCamera(id, cameraIdx)),
            onChangeFieldOfView: (id, fieldOfView) => dispatch(actionCreators.changeFieldOfView(id, fieldOfView)),
            onChangeAspectRatio: (id, aspectRatio) => dispatch(actionCreators.changeAspectRatio(id, aspectRatio)),
        };
    }
)(CameraPanel);

export default ConnectedCameraPanel;
