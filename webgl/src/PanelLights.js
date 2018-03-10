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
        case 'Lower Ambient':   if (currentScene.ambientLS) { [r, g, b] = currentScene.ambientLS.lowerHemisphereColor; } break;
        case 'Upper Ambient':   if (currentScene.ambientLS) { [r, g, b] = currentScene.ambientLS.upperHemisphereColor; } break;
        case 'Omni Directional':if (currentScene.omniDirLS) { [r, g, b] = currentScene.omniDirLS.color; } break;
        default: break;
        }
        this.setState({ option, r, g, b });
    }

    onSceneLoaded(loadedScene) {
        const currentScene = this.props.getCurrentScene();
        if (currentScene === loadedScene) {
            let r, g, b, option = currentScene.panelLS;
            switch (option) {
            case 'Lower Ambient': [r, g, b] = currentScene.ambientLS.lowerHemisphereColor; break;
            case 'Upper Ambient': [r, g, b] = currentScene.ambientLS.upperHemisphereColor; break;
            case 'Omni Directional':          [r, g, b] = currentScene.omniDirLS.color; break;
            default: break;
            }
            this.setState({ option, r, g, b });
        }
    }

    onChangeLight(event) {
        const currentScene = this.props.getCurrentScene();

        let r, g, b, option = event.target.value;
        switch (option) {
        case 'Lower Ambient': [r, g, b] = currentScene.ambientLS.lowerHemisphereColor; break;
        case 'Upper Ambient': [r, g, b] = currentScene.ambientLS.upperHemisphereColor; break;
        case 'Omni Directional':          [r, g, b] = currentScene.omniDirLS.color; break;
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
        case 'Lower Ambient': currentScene.ambientLS.lowerHemisphereColor[i] = v; break;
        case 'Upper Ambient': currentScene.ambientLS.upperHemisphereColor[i] = v; break;
        case 'Omni Directional':          currentScene.omniDirLS.color[i] = v; break;
        default: break;
        }

        this.setState({ [event.target.id]: v });
    }

    render() {
        const currentScene = this.props.getCurrentScene();

        if (currentScene && currentScene.ambientLS && currentScene.omniDirLS) {

            return <div id='LightPanel' className='CanvasPanel'> 
                <fieldset className='Fieldset'> 
                    <legend className='Legend'>Light Source</legend>
                    <select className='LightSelect' value={this.state.option} onChange={this.onChangeLight}>
                        <option>Lower Ambient</option>
                        <option>Upper Ambient</option>
                        <option>Omni Directional</option>
                    </select>
                </fieldset>

                <fieldset style={{borderColor:'red'}} className='Fieldset'> 
                    <legend style={{color: 'red'}} className='Legend'>
                        R <span>{(this.state.r * 255.0).toFixed(0)}</span>
                    </legend>
                    <input  id='r' className='Range' 
                            value={this.state.r * 255.0} 
                            type='range' 
                            min='0' 
                            max='255' 
                            onChange={this.onChangeRange} 
                            />
                </fieldset>

                <fieldset style={{borderColor:'green'}} className='Fieldset'> 
                    <legend style={{color: 'green'}} className='Legend'>
                        G <span>{(this.state.g * 255.0).toFixed(0)}</span>
                    </legend>
                    <input  id='g' 
                            className='Range' 
                            value={this.state.g * 255.0} 
                            type='range' 
                            min='0' max='255' 
                            onChange={this.onChangeRange} 
                            />
                </fieldset>

                <fieldset style={{borderColor:'blue'}} className='Fieldset'> 
                    <legend style={{color: 'blue'}} className='Legend'>
                        B <span>{(this.state.b * 255.0).toFixed(0)}</span>
                    </legend>
                    <input  id='b' 
                            className='Range' 
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