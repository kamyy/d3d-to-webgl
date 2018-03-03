import React, { Component } from 'react';
import './App.css';

export default class CanvasPanelCamera extends Component {
    constructor(props) {
        super(props);
        this.state = { 
            currentScene: props.getCurrentScene(),
            activeCamIdx: 0,
            sceneChanges: 0
        };
        this.onCanvasSceneChange = this.onCanvasSceneChange.bind(this);
        this.onSceneLoadFinished = this.onSceneLoadFinished.bind(this);
        this.onChange = this.onChange.bind(this);
        this.props.onRef(this);
    }

    onCanvasSceneChange() {
        const currentScene = this.props.getCurrentScene();
        this.setState({ 
            currentScene, 
            activeCamIdx: currentScene.activeCamIdx,
            sceneChanges: this.state.sceneChanges + 1
        });
    }

    onSceneLoadFinished(loadedScene) {
        const currentScene = this.props.getCurrentScene();
        if (currentScene === loadedScene) {
            this.setState({ 
                currentScene, 
                activeCamIdx: currentScene.activeCamIdx,
                sceneChanges: this.state.sceneChanges + 1
            });
        }
    }

    onChange(event) {
        const activeCamIdx = parseInt(event.target.value, 10);
        this.setState({ 
            activeCamIdx 
        });
        this.props.getCurrentScene().onChangeCamera(activeCamIdx);
    } 

    render() {
        const scene = this.props.getCurrentScene();

        if (scene && scene.cameras.length !== 0) {
            return <div id='canvas-panel-camera' className='canvas-panel'> {
                scene.cameras.map((cam, idx) => {
                    const id = `Camera ${idx + 1}`;
                    return <div key={id}> 
                        <input 
                            id={id}
                            type='radio'
                            value={idx}
                            checked={this.state.activeCamIdx === idx} 
                            onChange={this.onChange}
                            className='canvas-panel-camera-radio-button' 
                            />
                        <label htmlFor={id} className='canvas-panel-camera-radio-button-label'>{id}</label>
                    </div>
                })
            } </div>
        }

        return null;
    }
}
