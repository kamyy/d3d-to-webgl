import './App.css';
import { createStore } from 'redux';
import React, { Component } from 'react';

import Scene from './Scene';
import Vector1x4 from './Vector1x4';
import ShaderP3C3 from './ShaderP3C3';
import ShaderP3N3 from './ShaderP3N3';
import ShaderP3N3T2 from './ShaderP3N3T2';
import ShaderP3N3B3T2 from './ShaderP3N3B3T2';

import ConnectedScenePanel from './ConnectedScenePanel';
import ConnectedCameraPanel from './ConnectedCameraPanel';

import PanelRender from './PanelRender';
import PanelLights from './PanelLights';
import PanelMaterials from './PanelMaterials';

import actionCreators from '/Actions';
import appReducer from './Reducers';

export let GL         = null;
export let reduxStore = createStore(appReducer);
export let sceneArray = [ new Scene(0, 'hardwood'), new Scene(1, 'biplane'), new Scene(2, 'goku') ];

Object.defineProperty(sceneArray, 'curScene', {
    get: function() { return this[reduxStore.getState().curSceneId]; }
});

export default class App extends Component {
    constructor(props) {
        super(props);

        this.TXYZ_SCALAR = 0.01;
        this.RXYZ_SCALAR = 0.25;
        this.lButtonDown = false;
        this.rButtonDown = false;
        this.lx = 0;
        this.ly = 0;

        this.onMouseUp = this.onMouseUp.bind(this);
        this.onMouseMove = this.onMouseMove.bind(this);
        this.onMouseDown = this.onMouseDown.bind(this);
    }

    render() {
        return (
            <div className='App'>
                <canvas id='Canvas' width='1280' height='720'>Please use a browser that supports WebGL</canvas> 
                { 
                    (this.state.rootNode) ? null : <div className='Spinner'/> 
                }

                <div id='LHS'>
                    <ConnectedScenePanel/>
                    <ConnectedCameraPanel/>
                </div>

                <div id='Bottom'>
                    <PanelRender
                        getCurrentScene={this.getCurrentScene} 
                        onRef={panelRender => this.panelRender = panelRender} 
                        />
                </div>

                <div id='RHS'>
                    <PanelLights
                        getCurrentScene={this.getCurrentScene} 
                        onRef={panelLights => this.panelLights = panelLights} 
                        />
                    <PanelMaterials
                        getCurrentScene={this.getCurrentScene} 
                        onRef={panelMaterials => this.panelMaterials = panelMaterials} 
                        />
                </div>

                <hr/>

                <p className='ProjectInfo'>MIT License</p>
                <p className='ProjectInfo'><a href='https://github.com/kamyy/d3d-to-webgl'>Project @ GitHub</a></p>
                <p className='ProjectInfo'>Copyright &copy; 2018 <a href='mailto:kam.yin.yip@gmail.com'>Kam Y Yip</a></p>
            </div>
        );
    }

    componentDidMount() {
        this.canvas = document.getElementById('Canvas');
        GL = this.canvas.getContext("experimental-webgl", {
            depth: true,
            alpha: false,
            stencil: true
        });

        if (GL) {
            this.canvas.oncontextmenu = event => event.preventDefault(); // disable right click context menu
            this.canvas.onmousedown = this.onMouseDown;
            window.onmousemove = this.onMouseMove;
            window.onmouseup = this.onMouseUp;

            GL.depthFunc(GL.LESS);    // less than depth test
            GL.enable(GL.DEPTH_TEST); // enable depth testing
            GL.enable(GL.CULL_FACE);  // enable backface culling
            GL.enable(GL.BLEND);      // enable alpha blending
            GL.blendFunc(GL.SRC_ALPHA, GL.ONE_MINUS_SRC_ALPHA);

            GL.clearColor(0.392156899, 0.584313750, 0.929411829, 1.0); // cornflower blue
            GL.clearDepth(1.0); // depth buffer clear value
            GL.clearStencil(0); // stencil buffer clear value 

            GL.mapOfShaders = Object.freeze(new Map([
                ['P3C3', new ShaderP3C3(this.getCurrentScene)],
                ['P3N3', new ShaderP3N3(this.getCurrentScene)],
                ['P3N3T2', new ShaderP3N3T2(this.getCurrentScene)],
                ['P3N3B3T2', new ShaderP3N3B3T2(this.getCurrentScene)]
            ]));

            reduxStore.dispatch(actionCreators.changeCurScene(0));
        }
    }

    degreesToRadians(degrees) {
        return degrees * Math.PI / 180.0;
    }

    onMouseUp(event) {
        switch (event.button) {
        case 0: this.lButtonDown = false; break;
        case 2: this.rButtonDown = false; break;
        default: break;
        }
    }

    onMouseDown(event) {
        const rect = this.canvas.getBoundingClientRect();
        const x = event.clientX;
        const y = event.clientY;

        if (x > rect.left && x < rect.right && y > rect.top && y < rect.bottom) {
            switch (event.button) {
            case 0: this.lButtonDown = true; break;
            case 2: this.rButtonDown = true; break;
            default: break;
            }
            this.lx = x; 
            this.ly = y;
        } 
    }

    onMouseMove(event) {
        if (this.lButtonDown || 
            this.rButtonDown) {

            const scene = sceneArray.curScene;
            if (scene.activeCamera) {
                const x = event.clientX;
                const y = event.clientY;
                const camera = scene.activeCamera;
                const target = scene.activeCamera.parent;

                if ((this.lButtonDown && this.rButtonDown) || (this.lButtonDown && event.shiftKey)) { // dolly
                    camera.translate(new Vector1x4(0, (x - this.lx) * this.TXYZ_SCALAR, 0));
                    this.lx = x;
                    this.ly = y;
                } else if ((this.lButtonDown && event.ctrlKey) || this.rButtonDown) { // move
                    const dx = (this.lx - x) * this.TXYZ_SCALAR;
                    const dz = (y - this.ly) * this.TXYZ_SCALAR;
                    const dv = camera.mapPos(new Vector1x4(dx, 0, dz, 0), target);
                    target.translate(dv) // move target along own axes
                    this.lx = x;
                    this.ly = y;
                } else if (this.lButtonDown) { // rotate
                    target.rotateZ(this.degreesToRadians(this.lx - x) * this.RXYZ_SCALAR); // yaw camera target around it's own z-axis
                    camera.rotateX(this.degreesToRadians(this.ly - y) * this.RXYZ_SCALAR, target); // pitch around camera target's x-axis
                    this.lx = x;
                    this.ly = y;
                }
            }
        }
    }
}
