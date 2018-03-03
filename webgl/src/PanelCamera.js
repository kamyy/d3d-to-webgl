import React, { Component } from 'react';
import './App.css';

export default class PanelCamera extends Component {
    constructor(props) {
        super(props);

        const currentScene = props.getCurrentScene();
        const activeCamIdx = currentScene.activeCamIdx;
        const finishedLoad = 0;
        const fieldOfView  = 0;
        const aspectRatio  = 0;

        this.state = { currentScene, activeCamIdx, finishedLoad, fieldOfView, aspectRatio };
        this.onCanvasSceneChange = this.onCanvasSceneChange.bind(this);
        this.onSceneLoadFinished = this.onSceneLoadFinished.bind(this);
        this.onChangeFieldOfView = this.onChangeFieldOfView.bind(this);
        this.onChangeAspectRatio = this.onChangeAspectRatio.bind(this);
        this.onChangeCamera = this.onChangeCamera.bind(this);
        this.props.onRef(this);
    }

    onCanvasSceneChange() {
        const currentScene = this.props.getCurrentScene();
        const activeCamIdx = currentScene.activeCamIdx;
        const fieldOfView  = currentScene.activeCamera ? currentScene.activeCamera.fieldOfView : 0;
        const aspectRatio  = currentScene.activeCamera ? currentScene.activeCamera.aspectRatio : 0;
        this.setState({ currentScene, activeCamIdx, fieldOfView, aspectRatio });
    }

    onSceneLoadFinished(loadedScene) {
        const currentScene = this.props.getCurrentScene();
        if (currentScene === loadedScene) {
            const activeCamIdx = currentScene.activeCamIdx;
            const finishedLoad = this.state.finishedLoad + 1;
            const fieldOfView  = currentScene.activeCamera.fieldOfView;
            const aspectRatio  = currentScene.activeCamera.aspectRatio;
            this.setState({ activeCamIdx, finishedLoad, fieldOfView, aspectRatio });
        }
    }

    onChangeCamera(event) {
        const currentScene = this.props.getCurrentScene();
        const activeCamIdx = parseInt(event.target.value, 10);

        currentScene.onChangeCamera(activeCamIdx);

        const fieldOfView  = currentScene.activeCamera.fieldOfView;
        const aspectRatio  = currentScene.activeCamera.aspectRatio;

        this.setState({ activeCamIdx, fieldOfView, aspectRatio, });
    } 

    onChangeFieldOfView(event) {
        const activeCamera = this.props.getCurrentScene().activeCamera;
        const fieldOfView  = parseInt(event.target.value, 10);
        activeCamera.fieldOfView = fieldOfView;
        this.setState({ 
            fieldOfView 
        });
    }

    onChangeAspectRatio(event) {
        const activeCamera = this.props.getCurrentScene().activeCamera;
        const aspectRatio  = parseInt(event.target.value, 10) / 100.0;
        activeCamera.aspectRatio = aspectRatio;
        this.setState({ 
            aspectRatio 
        });
    }

    render() {
        const currentScene = this.props.getCurrentScene();

        if (currentScene && currentScene.cameras.length !== 0) {

            return <div id='camera-choice' className='canvas-panel'> 
                {
                    currentScene.cameras.map((cam, idx) => {
                        const id = `Camera ${idx + 1}`;
                        return <div key={id}> 
                            <input 
                                id={id}
                                type='radio'
                                className='camera-choice-button' 
                                value={idx}
                                checked={this.state.activeCamIdx === idx} 
                                onChange={this.onChangeCamera}
                                />
                            <label htmlFor={id} className='camera-choice-button-label'>{id}</label>
                        </div>
                    })
                }

                <br/>

                <fieldset className='camera-setup-fieldset'> 
                    <legend className='camera-setup-legend'>
                        Field of View <span className='camera-setup-value'>{this.state.fieldOfView.toFixed(0)} </span>
                    </legend>
                    <input
                        className='camera-setup-slider'
                        type='range'
                        min='15'
                        max='165'
                        value={this.state.fieldOfView} 
                        onChange={this.onChangeFieldOfView}
                        />
                </fieldset>

                <fieldset className='camera-setup-fieldset'> 
                    <legend className='camera-setup-legend'>
                        Aspect Ratio <span className='camera-setup-value'>{this.state.aspectRatio.toFixed(2)} </span>
                    </legend>
                    <input  
                        className='camera-setup-slider'
                        type='range' 
                        min='50' 
                        max='500'
                        value={this.state.aspectRatio * 100}
                        onChange={this.onChangeAspectRatio} 
                        />
                </fieldset>

            </div>
        }

        return null;
    }
}
