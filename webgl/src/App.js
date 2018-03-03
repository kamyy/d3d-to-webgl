import './App.css';
import React, { Component } from 'react';

import Scene from './Scene';
import Vector1x4 from './Vector1x4';
import ShaderP3C3 from './ShaderP3C3';
import ShaderP3N3 from './ShaderP3N3';
import ShaderP3N3T2 from './ShaderP3N3T2';
import ShaderP3N3B3T2 from './ShaderP3N3B3T2';

import CanvasPanelScene from './CanvasPanelScene';
import CanvasPanelCamera from './CanvasPanelCamera';
import CanvasPanelRender from './CanvasPanelRender';

export let GL = null;

export default class App extends Component {
    constructor(props) {
        super(props);

        this.TXYZ_SCALAR = 0.01;
        this.RXYZ_SCALAR = 0.25;
        this.buttonDown = false;
        this.lx = 0;
        this.ly = 0;

        this.onMouseUp = this.onMouseUp.bind(this);
        this.onMouseMove = this.onMouseMove.bind(this);
        this.onMouseDown = this.onMouseDown.bind(this);
        this.getCurrentScene = this.getCurrentScene.bind(this);
        this.onClickSceneButton = this.onClickSceneButton.bind(this);
        this.refCanvasPanelCamera = this.refCanvasPanelCamera.bind(this);
        this.refCanvasPanelRender = this.refCanvasPanelRender.bind(this);

        this.listOfScenes = [ 
            new Scene('hardwood', this.getCurrentScene), 
            new Scene('biplane', this.getCurrentScene), 
            new Scene('goku', this.getCurrentScene)
        ]
        this.currentScene = this.listOfScenes[0];

        this.state = {
            currentScene: this.currentScene
        }
    }

    refCanvasPanelCamera(canvasPanelCamera) {
        this.canvasPanelCamera = canvasPanelCamera;
    }

    refCanvasPanelRender(canvasPanelRender) {
        this.canvasPanelRender = canvasPanelRender;
    }

    render() {
        return (
            <div className="app">
                <canvas id='canvas' width='1280' height='720'> Please use a browser that supports WebGL </canvas> 

                <CanvasPanelScene 
                    listOfScenes={this.listOfScenes} 
                    getCurrentScene={this.getCurrentScene} 
                    onClick={this.onClickSceneButton} 
                    />

                <CanvasPanelCamera
                    getCurrentScene={this.getCurrentScene} 
                    onRef={this.refCanvasPanelCamera} 
                    />

                <CanvasPanelRender
                    getCurrentScene={this.getCurrentScene} 
                    onRef={this.refCanvasPanelRender} 
                    />
                <hr/>
                
                <div id='copyright'> 
                    <p>MIT License</p>
                    <p>Copyright &copy; 2018 <a href='mailto:kam.yin.yip@gmail.com'>Kam Y Yip</a></p>
                </div>
            </div>
        );
    }

    componentDidMount() {
        this.canvas = document.getElementById('canvas');
        GL = this.canvas.getContext("webgl", {
            depth: true,
            alpha: false,
            stencil: true
        });

        if (GL) {
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

            this.currentScene.loadScene(this.canvasPanelCamera.onSceneLoadFinished);
            this.currentScene.drawScene();
        }
    }

    getCurrentScene() {
        return this.currentScene;
    }

    degreesToRadians(degrees) {
        return degrees * Math.PI / 180.0;
    }

    onMouseUp(event) {
        if (event.button === 0) {
            this.buttonDown = false;
        }
    }

    onMouseDown(event) {
        const rect = this.canvas.getBoundingClientRect();
        const x = event.clientX;
        const y = event.clientY;

        if (event.button === 0 && x > rect.left && x < rect.right && y > rect.top && y < rect.bottom) {
            this.buttonDown = true; 
            this.lx = x; 
            this.ly = y;
        }
    }

    onMouseMove(event) {
        const x = event.clientX;
        const y = event.clientY;

        if (this.currentScene.activeCamera && this.buttonDown && (x !== this.lx || y !== this.ly)) {
            const camera = this.currentScene.activeCamera;
            const target = this.currentScene.activeCamera.parent;

            if (event.altKey) {
                target.rotateZ(this.degreesToRadians(this.lx - x) * this.RXYZ_SCALAR); // yaw camera target around it's own z-axis
                camera.rotateX(this.degreesToRadians(this.ly - y) * this.RXYZ_SCALAR, target); // pitch around camera target's x-axis
            } else if (event.shiftKey) {
                camera.translate(new Vector1x4(0, (x - this.lx) * this.TXYZ_SCALAR, 0));
            } else {
                const dx = (this.lx - x) * this.TXYZ_SCALAR;
                const dz = (y - this.ly) * this.TXYZ_SCALAR;
                const dv = camera.mapPos(new Vector1x4(dx, 0, dz, 0), target);
                target.translate(dv) // move target along own axes
            }
            this.lx = x;
            this.ly = y;
        }
    }

    onClickSceneButton(event) {
        const i = this.listOfScenes.findIndex(scene => scene.name === event.target.textContent);
        if (i > -1) {
            this.currentScene = this.listOfScenes[i];
            this.currentScene.loadScene(this.canvasPanelCamera.onSceneLoadFinished);
            this.currentScene.drawScene();

            this.setState({ currentScene: this.currentScene });
            this.canvasPanelCamera.onCanvasSceneChange();
            this.canvasPanelRender.onCanvasSceneChange();
        }
    }
}
