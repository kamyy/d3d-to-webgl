import './App.css';
import React, { Component } from 'react';

import Scene from './Scene';
import Vector1x4 from './Vector1x4';
import ShaderP3C3 from './ShaderP3C3';
import ShaderP3N3 from './ShaderP3N3';
import ShaderP3N3T2 from './ShaderP3N3T2';
import ShaderP3N3B3T2 from './ShaderP3N3B3T2';

import PanelScene from './PanelScene';
import PanelCamera from './PanelCamera';
import PanelRender from './PanelRender';
import PanelLights from './PanelLights';
import PanelMaterials from './PanelMaterials';

export let GL = null;

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
        this.onSceneLoaded = this.onSceneLoaded.bind(this);
        this.getCurrentScene = this.getCurrentScene.bind(this);
        this.onClickSceneButton = this.onClickSceneButton.bind(this);

        this.refPanelCamera = this.refPanelCamera.bind(this);
        this.refPanelRender = this.refPanelRender.bind(this);
        this.refPanelLights = this.refPanelLights.bind(this);
        this.refPanelMaterials = this.refPanelMaterials.bind(this);

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

    refPanelCamera(panelCamera) {
        this.panelCamera = panelCamera;
    }

    refPanelRender(panelRender) {
        this.panelRender = panelRender;
    }

    refPanelLights(panelLights) {
        this.panelLights = panelLights;
    }

    refPanelMaterials(panelMaterials) {
        this.panelMaterials = panelMaterials;
    }

    render() {
        return (
            <div className='App'>
                <canvas id='Canvas' width='1280' height='720'>Please use a browser that supports WebGL</canvas> 

                <div id='LHS'>
                    <PanelScene 
                        getCurrentScene={this.getCurrentScene} 
                        listOfScenes={this.listOfScenes} 
                        onClick={this.onClickSceneButton} 
                        />
                    <PanelCamera
                        getCurrentScene={this.getCurrentScene} 
                        onRef={this.refPanelCamera} 
                        />
                </div>

                <div id='Bottom'>
                    <PanelRender
                        getCurrentScene={this.getCurrentScene} 
                        onRef={this.refPanelRender} 
                        />
                </div>

                <div id='RHS'>
                    <PanelLights
                        getCurrentScene={this.getCurrentScene} 
                        onRef={this.refPanelLights} 
                        />
                        
                    <PanelMaterials
                        getCurrentScene={this.getCurrentScene} 
                        onRef={this.refPanelMaterials} 
                        />
                </div>
                
                <hr/>

                <p className='ProjectInfo'>MIT License</p>
                <p className='ProjectInfo'><a href='https://github.com/kamyy/d3d-to-webgl'>d3d-to-webgl @ GitHub</a></p>
                <p className='ProjectInfo'>Copyright &copy; 2018 <a href='mailto:kam.yin.yip@gmail.com'>Kam Y Yip</a></p>
            </div>
        );
    }

    componentDidMount() {
        this.canvas = document.getElementById('Canvas');
        GL = this.canvas.getContext("webgl", {
            depth: true,
            alpha: false,
            stencil: true
        });

        if (GL) {
            this.canvas.oncontextmenu = event => event.preventDefault();
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

            this.currentScene.loadScene(this.onSceneLoaded);
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
        if (this.currentScene.activeCamera) {
            const x = event.clientX;
            const y = event.clientY;
            const camera = this.currentScene.activeCamera;
            const target = this.currentScene.activeCamera.parent;

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

    onSceneLoaded(loadedScene) {
        this.panelCamera.onSceneLoaded(loadedScene);
        this.panelLights.onSceneLoaded(loadedScene);
        this.panelMaterials.onSceneLoaded(loadedScene);
    }

    onClickSceneButton(event) {
        const i = this.listOfScenes.findIndex(scene => scene.name === event.target.textContent);
        if (i > -1) {
            this.currentScene = this.listOfScenes[i];
            this.currentScene.loadScene(this.onSceneLoaded);
            this.currentScene.drawScene();

            this.setState({ currentScene: this.currentScene });
            this.panelCamera.onSceneChange();
            this.panelRender.onSceneChange();
            this.panelLights.onSceneChange();
            this.panelMaterials.onSceneChange();
        }
    }
}
