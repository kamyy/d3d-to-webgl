import './App.css';
import { createStore } from 'redux';
import { connect } from 'react-redux';
import React, { Component } from 'react';

import Scene from './Scene';
import Vector1x4 from './Vector1x4';
import ShaderP3C3 from './ShaderP3C3';
import ShaderP3N3 from './ShaderP3N3';
import ShaderP3N3T2 from './ShaderP3N3T2';
import ShaderP3N3B3T2 from './ShaderP3N3B3T2';

import ConnectedScenePanel from './ConnectedScenePanel';
import ConnectedCameraPanel from './ConnectedCameraPanel';
import ConnectedRenderPanel from './ConnectedRenderPanel';
import ConnectedLightsPanel from './ConnectedLightsPanel';
import ConnectedMaterialsPanel from './ConnectedMaterialsPanel';

import appReducer from './Reducers';

export let GL         = null;
export let reduxStore = createStore(appReducer);
export let sceneArray = [ new Scene(0, 'hardwood'), new Scene(1, 'biplane'), new Scene(2, 'goku') ];

Object.defineProperty(sceneArray, 'curScene', {
    get: function() { return this[reduxStore.getState().curSceneId]; }
});

class App extends Component {
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

        this.rootNode = null;
    }

    render() {
        return <div className='App'>
            <canvas id='Canvas' width='1280' height='720'>Please use a browser that supports WebGL</canvas> 
            { 
                (this.props.sceneState === null) ? <div className='Spinner'/> : null
            }

            <div id='LHS'>
                <ConnectedScenePanel/>
                <ConnectedCameraPanel/>
            </div>

            <div id='Bottom'>
                <ConnectedRenderPanel/>
            </div>

            <div id='RHS'>
                <ConnectedLightsPanel/>
                <ConnectedMaterialsPanel/>
            </div>

            <hr/>

            <p className='ProjectInfo'>MIT License</p>
            <p className='ProjectInfo'><a href='https://github.com/kamyy/d3d-to-webgl'>Project @ GitHub</a></p>
            <p className='ProjectInfo'>Copyright &copy; 2018 <a href='mailto:kam.yin.yip@gmail.com'>Kam Y Yip</a></p>

            <br/>

            <p className='MouseInfo'><span className='MouseInfoBold'>* Rotate</span> Left click + drag.</p>
            <p className='MouseInfo'><span className='MouseInfoBold'>* Translate</span> Right click + drag. Or ctrl + left click + drag.</p>
            <p className='MouseInfo'><span className='MouseInfoBold'>* Dolly In/Out</span> Left + right click + drag. Or shift + left click + drag.</p>
        </div>
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
                ['P3C3', new ShaderP3C3()],
                ['P3N3', new ShaderP3N3()],
                ['P3N3T2', new ShaderP3N3T2()],
                ['P3N3B3T2', new ShaderP3N3B3T2()]
            ]));

            sceneArray.curScene.loadScene();
            sceneArray.curScene.drawScene();
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

            const camera = sceneArray.curScene.activeCamera;
            if (camera) {
                const x = event.clientX;
                const y = event.clientY;
                const target = camera.parent;

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

const ConnectedApp = connect(
    function({ sceneArray, curSceneId }) { 
        return { 
            sceneState: sceneArray[curSceneId] 
        };
    }
)(App);

export default ConnectedApp;
