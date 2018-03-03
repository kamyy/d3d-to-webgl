import './App.css';
import React, { Component } from 'react';

/*
export function Select(props) {
    const { id, options, onChange, defaultIndex } = props;

    return <select  id={id} 
                    defaultValue={defaultIndex}
                    onChange={event => onChange(parseInt(event.target.value, 10))}> { 

        options.map( (opt, idx) => <option key={opt} value={idx}> {opt} </option> )

    } </select> 
}

class CameraPanel extends Component {
    constructor(props) {
        super(props);
        this.scene = props.scene;

        const activeCamera = this.scene.activeCamera;
        this.state = {
            fieldOfView: activeCamera.fieldOfView,
            aspectRatio: activeCamera.aspectRatio,
        };

        this.onChangeCameraSelect = this.onChangeCameraSelect.bind(this);
        this.onChangeFieldOfView = this.onChangeFieldOfView.bind(this);
        this.onChangeAspectRatio = this.onChangeAspectRatio.bind(this);
    }

    onChangeCameraSelect(index) {
        this.scene.onChangeCameraSelect(index);

        const activeCamera = this.scene.activeCamera;
        this.setState({
            fieldOfView: activeCamera.fieldOfView,
            aspectRatio: activeCamera.aspectRatio,
        });
    }

    onChangeFieldOfView(event) {
        const val = parseInt(event.target.value, 10);
        this.scene.activeCamera.fieldOfView = val;
        this.setState({ fieldOfView: val });
    }

    onChangeAspectRatio(event) {
        const val = parseInt(event.target.value, 10) / 100.0;
        this.scene.activeCamera.aspectRatio = val;
        this.setState({ aspectRatio: val });
    }

    render() {
        const fieldOfViewId = `${this.scene.name}-FieldOfView`;
        const aspectRatioId = `${this.scene.name}-AspectRatio`;

        return (
            <fieldset className='accordion-fieldset'> 
                <legend>Camera</legend>
                <div> 
                    <label htmlFor={fieldOfViewId}>Field of View</label>
                    <input
                        type='range'
                        id={fieldOfViewId}
                        min='30'
                        max='120'
                        value={this.state.fieldOfView} 
                        onChange={this.onChangeFieldOfView}
                        />
                </div>

                <div> 
                    <label htmlFor={aspectRatioId}>Aspect Ratio</label>
                    <input  
                        type='range' 
                        id={aspectRatioId} 
                        min='50' 
                        max='500'
                        value={this.state.aspectRatio * 100}
                        onChange={this.onChangeAspectRatio} 
                        />
                </div>
            </fieldset>
        )
    }
}

export class SceneUI extends Component {
    constructor (props) {
        super(props);

        this.state = {
            isLoaded: false
        }
        this.scene = props.scene;
        this.scene.onSceneLoaded = this.onSceneLoaded.bind(this);
    }

    onSceneLoaded() {
        this.setState({
            isLoaded: true
        });
    }

    render() {
        if (this.state.isLoaded) {
            return (
                <div>
                    <CameraPanel scene={this.scene}/>
                </div>
            )
        }
        return null;
    }
}

*/