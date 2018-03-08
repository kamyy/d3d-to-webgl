import React, { Component } from 'react';
import './App.css';

export default class PanelCamera extends Component {
    constructor(props) {
        super(props);

        const currentScene = props.getCurrentScene();
        const activeCamIdx = currentScene.activeCamIdx;
        const fieldOfView  = 0;
        const aspectRatio  = 0;

        this.state = { activeCamIdx, fieldOfView, aspectRatio };
        this.onSceneChange = this.onSceneChange.bind(this);
        this.onSceneLoaded = this.onSceneLoaded.bind(this);
        this.onChangeFieldOfView = this.onChangeFieldOfView.bind(this);
        this.onChangeAspectRatio = this.onChangeAspectRatio.bind(this);
        this.onChangeCamera = this.onChangeCamera.bind(this);
        this.props.onRef(this);
    }

    onSceneChange() {
        const currentScene = this.props.getCurrentScene();
        const activeCamIdx = currentScene.activeCamIdx;
        const fieldOfView  = currentScene.activeCamera ? currentScene.activeCamera.fieldOfView : 0;
        const aspectRatio  = currentScene.activeCamera ? currentScene.activeCamera.aspectRatio : 0;
        this.setState({ activeCamIdx, fieldOfView, aspectRatio });
    }

    onSceneLoaded(loadedScene) {
        const currentScene = this.props.getCurrentScene();
        if (currentScene === loadedScene) {
            const activeCamIdx = currentScene.activeCamIdx;
            const fieldOfView  = currentScene.activeCamera.fieldOfView;
            const aspectRatio  = currentScene.activeCamera.aspectRatio;
            this.setState({ activeCamIdx, fieldOfView, aspectRatio });
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
            return <div id='camera-setup' className='canvas-panel'> 
                {  
                    (currentScene.cameras.length > 1) ?
                        <div style={{marginBottom: '8px'}}> {
                            currentScene.cameras.map((cam, idx) => {
                                const id = `Camera ${idx + 1}`;
                                return <div key={id}> 
                                    <input 
                                        id={id}
                                        type='radio'
                                        className='camera-choice-button' 
                                        value={idx}
                                        checked={currentScene.activeCamIdx === idx} 
                                        onChange={this.onChangeCamera}
                                        />
                                    <label htmlFor={id} className='camera-choice-button-label'>{id}</label>
                                </div>
                            })
                        } </div>
                        :
                        <div style={{marginBottom: '4px'}}/>
                }

                <fieldset className='camera-setup-fieldset'> 
                    <legend className='camera-setup-legend'>
                        Field of View <span className='camera-setup-value'>{currentScene.activeCamera.fieldOfView.toFixed(0)} </span>
                    </legend>
                    <input
                        className='camera-setup-range'
                        type='range'
                        min='15'
                        max='165'
                        value={currentScene.activeCamera.fieldOfView.toFixed(0)} 
                        onChange={this.onChangeFieldOfView}
                        />
                </fieldset>

                <fieldset className='camera-setup-fieldset'> 
                    <legend className='camera-setup-legend'>
                        Aspect Ratio <span className='camera-setup-value'>{currentScene.activeCamera.aspectRatio.toFixed(2)} </span>
                    </legend>
                    <input  
                        className='camera-setup-range'
                        type='range' 
                        min='50' 
                        max='500'
                        value={currentScene.activeCamera.aspectRatio * 100}
                        onChange={this.onChangeAspectRatio} 
                        />
                </fieldset>
            </div>
        }

        return null;
    }
}
