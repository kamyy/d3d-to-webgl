import React, { Component } from 'react';
import './App.css';

export default class PanelRender extends Component {
    constructor(props) {
        super(props);

        this.state = { 
            drawWirefrm: props.getCurrentScene().drawWirefrm,
            drawNormals: props.getCurrentScene().drawNormals,
        };
        this.onCanvasSceneChange = this.onCanvasSceneChange.bind(this);
        this.onChangeWirefrm = this.onChangeWirefrm.bind(this);
        this.onChangeNormals = this.onChangeNormals.bind(this);
        this.props.onRef(this);
    }

    onCanvasSceneChange() {
        const scene = this.props.getCurrentScene();
        this.setState({ 
            drawWirefrm: scene.drawWirefrm,
            drawNormals: scene.drawNormals,
        });
    }

    onChangeWirefrm(event) {
        const drawWirefrm = event.target.checked;
        this.setState( {
            drawWirefrm
        });
        this.props.getCurrentScene().onChangeWirefrm(drawWirefrm);
    }

    onChangeNormals(event) {
        const drawNormals = event.target.checked;
        this.setState( {
            drawNormals 
        });
        this.props.getCurrentScene().onChangeNormals(drawNormals);
    }

    render() {
        const scene = this.props.getCurrentScene();
        const id0 = 'wirefrmCheckbox';
        const id1 = 'normalsCheckbox';

        return <div>
            <span id='wirefrm-choice' className='canvas-panel'>
                <input id={id0} type='checkbox' checked={scene.drawWirefrm} onChange={this.onChangeWirefrm} className='checkbox' />
                <label htmlFor={id0} className='checkbox-label'>Primitive Edges Only</label>
            </span>
            <span id='normals-choice' className='canvas-panel'>
                <input id={id1} type='checkbox' checked={scene.drawNormals} onChange={this.onChangeNormals} className='checkbox' />
                <label htmlFor={id1} className='checkbox-label'>Vertex Normals</label>
            </span>
        </div>
    }
}