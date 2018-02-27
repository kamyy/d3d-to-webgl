import React, { Component } from 'react';

export function SelectComponent(props) {
    const {
        id,
        options, 
        onChange,
        defaultIndex
    } = props;

    return (
        <select id={id} 
                defaultValue={defaultIndex}
                onChange={ event => onChange(parseInt(event.target.value, 10)) }> 
            { 
                options.map( (opt, idx) => {
                    return <option key={idx} value={idx}> {opt} </option> 
                })
            } 
        </select> 
    )
}

export function CheckboxComponent(props) {
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

export class SceneComponent extends Component {
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
            const options = ['One', 'Two', 'Three'].slice(0, this.scene.cameras.length);
            const cameraSelectId = `${this.scene.name}-CameraSelect`;
            const renderEdgesId = `${this.scene.name}-DrawWirefrmCheckbox`;
            const drawNormalsId = `${this.scene.name}-DrawNormalsCheckbox`;
        
            return (
                <div>
                    <fieldset> 
                        <legend>Cameras</legend>
                        <label htmlFor={cameraSelectId}>Choose Camera</label>
                        <SelectComponent id={cameraSelectId} options={options} onChange={this.scene.onChangeCameraSelect} />
                    </fieldset>

                    <fieldset> 
                        <legend>Rendering</legend>
                        <div> 
                            <CheckboxComponent id={renderEdgesId} onChange={this.scene.onChangeDrawWirefrmCheckbox} />
                            <label htmlFor={renderEdgesId}>Render Edges</label>
                        </div> 
                        <div> 
                            <CheckboxComponent id={drawNormalsId} onChange={this.scene.onChangeDrawNormalsCheckbox} />
                            <label htmlFor={drawNormalsId}>Draw Normals</label>
                        </div> 
                    </fieldset>
                </div>
            )
        }

        return <div/>
    }
}
