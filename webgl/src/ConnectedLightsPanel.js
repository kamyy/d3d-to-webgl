import React from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';

import './App.css';
import { sceneArray } from './App.js';
import { actionCreators } from './Actions.js';

function LightsPanel(props) {
    const { 
        sceneState, 
        curSceneId, 
        onChangeLightSource, 
        onChangeR, 
        onChangeG,
        onChangeB,
    } = props;

    if (sceneState) {
        let r, g, b;

        sceneArray.curScene.requestDrawScene();

        switch(sceneState.currentLS) {
            case 'Omni Directional': [r, g, b] = sceneState.omniDirLS; break;
            case 'Lower Ambient'   : [r, g, b] = sceneState.lowerAmbientLS; break;
            case 'Upper Ambient'   : [r, g, b] = sceneState.upperAmbientLS; break;
            default: r = g = b = 0; break;
        }

        return <div id='LightPanel' className='CanvasPanel'>
            <fieldset className='Fieldset'> 
                <legend className='Legend'>Light Source</legend>
                <select className='LightSelect' value={sceneState.currentLS} onChange={event => onChangeLightSource(curSceneId, event.target.value)}>
                    <option>Omni Directional</option>
                    <option>Lower Ambient</option>
                    <option>Upper Ambient</option>
                </select>
            </fieldset>

            <fieldset style={{borderColor:'red'}} className='Fieldset'>
                <legend style={{color: 'red'}} className='Legend'>
                    R <span>{(r * 255.0).toFixed(0)}</span>
                </legend>
                <input  className='Range' type='range' min='0' max='255' value={r * 255.0}
                        onChange={event => onChangeR(curSceneId, event.target.value / 255.0)}
                        />
            </fieldset>

            <fieldset style={{borderColor:'green'}} className='Fieldset'>
                <legend style={{color: 'green'}} className='Legend'>
                    G <span>{(g * 255.0).toFixed(0)}</span>
                </legend>
                <input  className='Range' type='range' min='0' max='255' value={g * 255.0}
                        onChange={event => onChangeG(curSceneId, event.target.value / 255.0)}
                        />
            </fieldset>

            <fieldset style={{borderColor:'blue'}} className='Fieldset'>
                <legend style={{color: 'blue'}} className='Legend'>
                    B <span>{(b * 255.0).toFixed(0)}</span>
                </legend>
                <input  className='Range' type='range' min='0' max='255' value={b * 255.0}
                        onChange={event => onChangeB(curSceneId, event.target.value / 255.0)}
                        />
            </fieldset>
        </div>
    }
    return null;
}

LightsPanel.propTypes = {
    sceneState: PropTypes.object,
    curSceneId: PropTypes.number.isRequired,
    onChangeLightSource: PropTypes.func.isRequired,
    onChangeR: PropTypes.func.isRequired,
    onChangeG: PropTypes.func.isRequired,
    onChangeB: PropTypes.func.isRequired,
};

const ConnectedLightsPanel = connect(
    function({ sceneArray, curSceneId }) { 
        return { 
            sceneState: sceneArray[curSceneId], curSceneId 
        };
    },
    function(dispatch) { 
        return { 
            onChangeLightSource: (id, lightSource) => dispatch(actionCreators.changeLightSource(id, lightSource)),
            onChangeR: (id, value) => dispatch(actionCreators.changeR(id, value)),
            onChangeG: (id, value) => dispatch(actionCreators.changeG(id, value)),
            onChangeB: (id, value) => dispatch(actionCreators.changeB(id, value)),
        };
    }
)(LightsPanel);

export default ConnectedLightsPanel;
