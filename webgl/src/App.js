import React, { Component } from 'react';
import './App.css';

import AmbientLS from './AmbientLS';
import OmniDirLS from './OmniDirLS';
import RefFrame from './RefFrame';
import Camera from './Camera';
import Model from './Model';

import RenderUI from './RenderUI';
import Render3D from './Render3D';
import { g_GL } from './Render3D';

export const g_scene = {
    cameras: [],
    rootNode: null,
    cameraIdx: 0,
    mirrorCam: null,
    mirrorObj: null,
    ambientLS: new AmbientLS([0.15, 0.15, 0.15], [0.15, 0.15, 0.15]),
    omniDirLS: null,
    alphaPieces: [],
    drawWirefrm: false,
    drawNormals: false,

    mapOfShaders: new Map(),
    mapOfTextures: new Map(),
    mapOfMaterials: new Map(),

    get activeCamera() { 
        return this.cameras[this.cameraIdx];
    },
    set activeCamera(cam) {
        this.cameras[this.cameraIdx] = cam;
    },

    get activeCamIdx() { 
        return this.cameraIdx;
    },

    set activeCamIdx(idx) { 
        this.cameraIdx = idx;
    },

    loadScene: function(url) {
        this.mapOfTextures.clear();
        this.mapOfMaterials.clear();

        if (g_GL) {
            const initTextures = (textures) => {
                for (let text of textures) {
                    const texture = g_GL.createTexture();
                    this.mapOfTextures.set(text.name, texture);
                    g_GL.bindTexture(g_GL.TEXTURE_2D, texture);

                    g_GL.texImage2D(
                        g_GL.TEXTURE_2D, 
                        0, 
                        g_GL.RGBA, 
                        1, 
                        1, 
                        0, 
                        g_GL.RGBA, 
                        g_GL.UNSIGNED_BYTE, 
                        new Uint8Array([0, 0, 255, 255])
                    );

                    const image = new Image();
                    image.onload = () => {
                        g_GL.bindTexture(g_GL.TEXTURE_2D, this.mapOfTextures.get(text.name));
                        if (text.hasAlpha) {
                            g_GL.texImage2D(g_GL.TEXTURE_2D, 0, g_GL.RGBA, g_GL.RGBA, g_GL.UNSIGNED_BYTE, image);
                        } else {
                            g_GL.texImage2D(g_GL.TEXTURE_2D, 0, g_GL.RGB,  g_GL.RGB,  g_GL.UNSIGNED_BYTE, image);
                        }
                        g_GL.generateMipmap(g_GL.TEXTURE_2D);
                    };

                    image.crossOrigin = 'anonymous';
                    image.src = '/textures/' + text.name + '.png';
                }
            };

            const initMaterials = (materials)  => {
                for (let mat of materials) {
                    this.mapOfMaterials.set(mat.name, mat);
                    mat.textures.forEach((t, i) => {
                        if (t) {
                            mat.textures[i] = this.mapOfTextures.get(t); 
                        }
                    });
                    mat.shader = this.mapOfShaders.get(mat.shaderProgram);
                }
            };

            const initSceneGraph = (node, parent = null)  => {
                let refFrame = null;

                switch(node.nodeType) {
                case 'OmniDirLS':
                    refFrame = new OmniDirLS(parent, node); 
                    this.omniDirLS = refFrame;
                    break;
                case 'RefFrame':
                    refFrame = new RefFrame(parent, node); 
                    break;
                case 'Camera':
                    refFrame = new Camera(parent, node);
                    this.cameras.push(refFrame); 
                    break;
                case 'MirrorCamera':
                    refFrame = new Camera(parent, node);
                    this.mirrorCam = refFrame; 
                    break;
                case 'Model':
                    refFrame = new Model(parent, node);
                    if (refFrame.isFloor()) {
                        this.mirrorObj = refFrame; 
                    }
                    break;
                default:
                    break;
                }

                if (node.hasOwnProperty('children')) {
                    for (let child of node.children) {
                        initSceneGraph(child, refFrame);
                    }
                }

                if (!parent && refFrame) {
                    this.rootNode = refFrame;
                }
            };

            const request = new XMLHttpRequest();
            request.overrideMimeType("application/json");
            request.onreadystatechange = () => {
                if (request.readyState === 4 && request.status === 200) {
                    const scene = JSON.parse(request.responseText);
                    initTextures(scene.textures);
                    initMaterials(scene.materials);
                    initSceneGraph(scene.sceneRoot);
                }
            };
            request.open('GET', url, true);
            request.send();                    
        }
    },
};

export default class App extends Component {
    render() {
        return  <div className="App">
            <Render3D id='render3D' scene={g_scene}/>
            <RenderUI id='renderUI' scene={g_scene}/> 
        </div>
    }
}
