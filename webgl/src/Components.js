import React, { Component } from 'react';

export function SelectUI(props) {
    const {
        id,
        options, 
        onChange,
        defaultIndex
    } = props;

    return (
        <select id={id} 
                defaultValue={defaultIndex}
                onChange={event => onChange(parseInt(event.target.value, 10))}> 
            { 
                options.map( (opt, idx) => {
                    return <option key={opt} value={idx}> {opt} </option> 
                })
            } 
        </select> 
    )
}

export function CheckboxUI(props) {
    const {
        id,
        onChange
    } = props;

    return (
        <input id={id}
               key={id}
               type='checkbox'
               onChange={event => onChange(event.target.checked) }>
        </input>
    )
}

class CameraUI extends Component {
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
        const val = parseInt(event.target.value, 10);
        this.scene.activeCamera.aspectRatio = val;
        this.setState({ aspectRatio: val });
    }

    render() {
        const cameraSelectOptions = ['One', 'Two', 'Three', 'Four', 'Five'].slice(0, this.scene.cameras.length);
        const cameraSelectId = `${this.scene.name}-CameraSelect`;
        const fieldOfViewId = `${this.scene.name}-FieldOfView`;
        const aspectRatioId = `${this.scene.name}-AspectRatio`;

        return (
            <fieldset> 
                <legend>Cameras</legend>

                <label htmlFor={cameraSelectId}>Camera</label>
                <SelectUI   id={cameraSelectId} 
                            options={cameraSelectOptions} 
                            onChange={this.onChangeCameraSelect} />

                <div> 
                    <label htmlFor={fieldOfViewId}>Field of View</label>
                    <input
                        type='range'
                        id={fieldOfViewId}
                        min='30'
                        max='120'
                        step='10'
                        value={this.state.fieldOfView} 
                        onChange={this.onChangeFieldOfView}
                        />
                </div>

                <div> 
                    <label htmlFor={aspectRatioId}>Aspect Ratio</label>
                    <input  
                        type='range' 
                        id={aspectRatioId} 
                        min='0.5' 
                        max='5'
                        step='0.5'
                        value={this.state.aspectRatio}
                        onChange={this.onChangeAspectRatio} 
                        />
                </div>
            </fieldset>
        )
    }
}

function RenderingUI(props) {
    const {
        scene
    } = props;

    const renderEdgesId = `${scene.name}-DrawWirefrmCheckbox`;
    const drawNormalsId = `${scene.name}-DrawNormalsCheckbox`;
        
    return (
        <fieldset> 
            <legend>Rendering</legend>
            <CheckboxUI id={renderEdgesId} onChange={scene.onChangeDrawWirefrmCheckbox} />
            <label htmlFor={renderEdgesId}>Wireframe</label>
            <CheckboxUI id={drawNormalsId} onChange={scene.onChangeDrawNormalsCheckbox} />
            <label htmlFor={drawNormalsId}>Normals</label>
        </fieldset>
    )
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
                    <CameraUI scene={this.scene}/>
                    <RenderingUI scene={this.scene}/>
                </div>
            )
        }
        return null;
    }
}
