import React, { Component } from 'react';
import './App.css';

export default class PanelLights extends Component {
    constructor(props) {
        super(props);

        const currentScene = props.getCurrentScene();
        let lowerHemisphereColor = '#000000';
        let upperHemisphereColor = '#000000';

        if (currentScene) {
            lowerHemisphereColor = this.toHexString(currentScene.ambientLS.lowerHemisphereColor);
            upperHemisphereColor = this.toHexString(currentScene.ambientLS.upperHemisphereColor);
        }

        this.state = { 
            lowerHemisphereColor, 
            upperHemisphereColor, 
        };

        this.onSceneChange = this.onSceneChange.bind(this);
        this.onSceneLoaded = this.onSceneLoaded.bind(this);
        this.onInputLowerHemisphereColor = this.onInputLowerHemisphereColor.bind(this);
        this.onInputUpperHemisphereColor = this.onInputUpperHemisphereColor.bind(this);
        this.props.onRef(this);
    }

    toHexString(color) {
        let r = Math.floor((color[0] * 255.0 + 0.5)).toString(16);
        let g = Math.floor((color[1] * 255.0 + 0.5)).toString(16);
        let b = Math.floor((color[2] * 255.0 + 0.5)).toString(16);
        if (r.length < 2) { r = '0' + r; }
        if (g.length < 2) { g = '0' + g; }
        if (b.length < 2) { b = '0' + b; }
        return `#${r}${g}${b}`;
    }

    onSceneChange() {
        const currentScene = this.props.getCurrentScene();
        const lowerHemisphereColor = this.toHexString(currentScene.ambientLS.lowerHemisphereColor);
        const upperHemisphereColor = this.toHexString(currentScene.ambientLS.upperHemisphereColor);
        this.setState({ 
            lowerHemisphereColor,
            upperHemisphereColor,
        });
    }

    onSceneLoaded(loadedScene) {
        const currentScene = this.props.getCurrentScene();
        if (currentScene === loadedScene) {
            const lowerHemisphereColor = this.toHexString(currentScene.ambientLS.lowerHemisphereColor);
            const upperHemisphereColor = this.toHexString(currentScene.ambientLS.upperHemisphereColor);
            this.setState({ 
                lowerHemisphereColor,
                upperHemisphereColor,
            });
        }
    }

    toRgbArray(hexString) {
        let r = parseInt(hexString.slice(1, 3), 16) / 255.0;
        let g = parseInt(hexString.slice(3, 5), 16) / 255.0;
        let b = parseInt(hexString.slice(5, 7), 16) / 255.0;
        return [r, g, b];
    }

    onInputLowerHemisphereColor(event) {
        const lowerHemisphereColor = this.toRgbArray(event.target.value);
        const ambientLS = this.props.getCurrentScene().ambientLS;
        ambientLS.lowerHemisphereColor = lowerHemisphereColor;
        this.setState({lowerHemisphereColor});
    }

    onInputUpperHemisphereColor(event) {
        const upperHemisphereColor = this.toRgbArray(event.target.value);
        const ambientLS = this.props.getCurrentScene().ambientLS;
        ambientLS.upperHemisphereColor = upperHemisphereColor;
        this.setState({upperHemisphereColor});
    }

    render() {
        const currentScene = this.props.getCurrentScene();

        if (currentScene && currentScene.ambientLS && currentScene.omniDirLS) {

            return <div id='lights' className='canvas-panel'> 
                <fieldset className='lights-setup-fieldset'> 
                    <legend className='lights-setup-legend'>Ambient Lower Sphere</legend>
                    <input 
                        type='color' 
                        className='lights-setup-button' 
                        value={this.toHexString(currentScene.ambientLS.lowerHemisphereColor)}
                        onInput={this.onInputLowerHemisphereColor}
                        />
                </fieldset>

                <fieldset className='lights-setup-fieldset'> 
                    <legend className='lights-setup-legend'>Ambient Upper Sphere</legend>
                    <input 
                        type='color' 
                        className='lights-setup-button' 
                        value={this.toHexString(currentScene.ambientLS.upperHemisphereColor)}
                        onInput={this.onInputUpperHemisphereColor}
                        />
                </fieldset>
            </div>
        }

        return null;
    }
}