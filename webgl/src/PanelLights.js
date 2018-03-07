import React, { Component } from 'react';
import './App.css';

export default class PanelLights extends Component {
    constructor(props) {
        super(props);

        this.state = { option: props.getCurrentScene().panelLS, r: 0, g: 0, b: 0 };
        this.onSceneChange = this.onSceneChange.bind(this);
        this.onSceneLoaded = this.onSceneLoaded.bind(this);
        this.onChangeLight = this.onChangeLight.bind(this);
        this.onChangeRange = this.onChangeRange.bind(this);
        this.props.onRef(this);
    }

    onSceneChange() {
        const currentScene = this.props.getCurrentScene();
        let r = 0, g = 0, b = 0, option = currentScene.panelLS;
        switch (option) {
        case 'Lower Ambient Light': if (currentScene.ambientLS) { [r, g, b] = currentScene.ambientLS.lowerHemisphereColor; } break;
        case 'Upper Ambient Light': if (currentScene.ambientLS) { [r, g, b] = currentScene.ambientLS.upperHemisphereColor; } break;
        case 'Omni Light':          if (currentScene.omniDirLS) { [r, g, b] = currentScene.omniDirLS.color; } break;
        default: break;
        }
        this.setState({ option, r, g, b });
    }

    onSceneLoaded(loadedScene) {
        const currentScene = this.props.getCurrentScene();
        if (currentScene === loadedScene) {
            let r, g, b, option = currentScene.panelLS;
            switch (option) {
            case 'Lower Ambient Light': [r, g, b] = currentScene.ambientLS.lowerHemisphereColor; break;
            case 'Upper Ambient Light': [r, g, b] = currentScene.ambientLS.upperHemisphereColor; break;
            case 'Omni Light':          [r, g, b] = currentScene.omniDirLS.color; break;
            default: break;
            }
            this.setState({ option, r, g, b });
        }
    }

    onChangeLight(event) {
        const currentScene = this.props.getCurrentScene();

        let r, g, b, option = event.target.value;
        switch (option) {
        case 'Lower Ambient Light': [r, g, b] = currentScene.ambientLS.lowerHemisphereColor; break;
        case 'Upper Ambient Light': [r, g, b] = currentScene.ambientLS.upperHemisphereColor; break;
        case 'Omni Light':          [r, g, b] = currentScene.omniDirLS.color; break;
        default: break;
        }
        this.setState({ option, r, g, b });
        currentScene.panelLS = option;
    }

    onChangeRange(event) {
        const currentScene = this.props.getCurrentScene();
        let v = parseInt(event.target.value, 10) / 255.0;
        let i = 0;

        switch (event.target.id) {
        case 'r': i = 0; break;
        case 'g': i = 1; break;
        case 'b': i = 2; break;
        default: break;
        } 

        switch (this.state.option) {
        case 'Lower Ambient Light': currentScene.ambientLS.lowerHemisphereColor[i] = v; break;
        case 'Upper Ambient Light': currentScene.ambientLS.upperHemisphereColor[i] = v; break;
        case 'Omni Light':          currentScene.omniDirLS.color[i] = v; break;
        default: break;
        }

        this.setState({ [event.target.id]: v });
    }

    render() {
        const currentScene = this.props.getCurrentScene();

        if (currentScene && currentScene.ambientLS && currentScene.omniDirLS) {

            return <div id='lights' className='canvas-panel'> 
                <select className='lights-setup-select' value={this.state.option} onChange={this.onChangeLight}>
                    <option>Lower Ambient Light</option>
                    <option>Upper Ambient Light</option>
                    <option>Omni Light</option>
                </select>

                <fieldset style={{color: 'red', borderColor:'red'}} className='lights-setup-fieldset'> 
                    <legend style={{color: 'red'}} className='lights-setup-legend'>
                        R <span>{(this.state.r * 255.0).toFixed(0)}</span>
                    </legend>
                    <input  id='r' className='lights-setup-range' 
                            value={this.state.r * 255.0} 
                            type='range' 
                            min='0' 
                            max='255' 
                            onChange={this.onChangeRange} 
                            />
                </fieldset>

                <fieldset style={{color: 'green', borderColor:'green'}} className='lights-setup-fieldset'> 
                    <legend style={{color: 'green'}} className='lights-setup-legend'>
                        G <span>{(this.state.g * 255.0).toFixed(0)}</span>
                    </legend>
                    <input  id='g' 
                            className='lights-setup-range' 
                            value={this.state.g * 255.0} 
                            type='range' 
                            min='0' max='255' 
                            onChange={this.onChangeRange} 
                            />
                </fieldset>

                <fieldset style={{color: 'blue', borderColor:'blue'}} className='lights-setup-fieldset'> 
                    <legend style={{color: 'blue'}} className='lights-setup-legend'>
                        B <span>{(this.state.b * 255.0).toFixed(0)}</span>
                    </legend>
                    <input  id='b' 
                            className='lights-setup-range' 
                            value={this.state.b * 255.0} 
                            type='range' 
                            min='0' 
                            max='255' 
                            onChange={this.onChangeRange} 
                            />
                </fieldset>
            </div>
        }

        return null;
    }
}