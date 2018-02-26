import './App.css';
import React, { Component } from 'react';

import Model from './Model';
import Camera from './Camera';
import RefFrame from './RefFrame';
import AmbientLS from './AmbientLS';
import OmniDirLS from './OmniDirLS';
import Vector1x4 from './Vector1x4';
import Matrix4x4 from './Matrix4x4';
import ShaderP3C3 from './ShaderP3C3';
import ShaderP3N3 from './ShaderP3N3';
import ShaderP3N3T2 from './ShaderP3N3T2';
import ShaderP3N3B3T2 from './ShaderP3N3B3T2';

const DRAW = Object.freeze({
    MIRROR: Symbol("mirror"),
    PIECES: Symbol("pieces")
});

export let GL = null;

export default class App extends Component {
    constructor(props) {
        super(props);

        this.TXYZ_SCALAR = 0.01;
        this.RXYZ_SCALAR = 0.25;
        this.buttonDown = false;
        this.lx = 0;
        this.ly = 0;

        this.cameras = [];
        this.rootNode = null;
        this.cameraIdx = 0;
        this.mirrorCam = null;
        this.mirrorObj = null;
        this.ambientLS = null;
        this.omniDirLS = null;
        this.alphaPieces = [];
        this.drawWirefrm = false;
        this.drawNormals = false;
        this.mapOfShaders = new Map();
        this.mapOfTextures = new Map();
        this.mapOfMaterials = new Map();
        this.translucentPieces = [];

        this.drawScene = this.drawScene.bind(this);
        this.onMouseUp = this.onMouseUp.bind(this);
        this.onMouseMove = this.onMouseMove.bind(this);
        this.onMouseDown = this.onMouseDown.bind(this);
        this.cacheTranslucentPiece = this.cacheTranslucentPiece.bind(this);
    }

    render() {
        return <div className="App">
            <canvas id='canvas' width='1280' height='720'>Please use a browser that supports WebGL</canvas>
        </div>
    }

    degreesToRadians(degrees) {
        return degrees * Math.PI / 180.0;
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

        if (this.buttonDown && (x !== this.lx || y !== this.ly)) {
            const camera = this.activeCamera;
            const target = this.activeCamera.parent;

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

    onMouseUp(event) {
        if (event.button === 0) {
            this.buttonDown = false;
        }
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

            this.mapOfShaders = Object.freeze(new Map([
                ['P3C3',     new ShaderP3C3(this)], 
                ['P3N3',     new ShaderP3N3(this)], 
                ['P3N3T2',   new ShaderP3N3T2(this)],
                ['P3N3B3T2', new ShaderP3N3B3T2(this)]
            ]));

            this.ambientLS = new AmbientLS([0.15, 0.15, 0.15], [0.15, 0.15, 0.15]);

            this.loadScene('/json/hardwood.json');
            this.drawScene();
        }
    }

    get activeCamera() { 
        return this.cameras[this.cameraIdx];
    }

    set activeCamera(cam) {
        this.cameras[this.cameraIdx] = cam;
    }

    get activeCamIdx() { 
        return this.cameraIdx;
    }

    set activeCamIdx(idx) { 
        this.cameraIdx = idx;
    }

    initTextures(textures) {
        const onLoad = (tex, png) => {
            return () => {
                GL.bindTexture(GL.TEXTURE_2D, this.mapOfTextures.get(tex.name));
                if (tex.hasAlpha) {
                    GL.texImage2D(GL.TEXTURE_2D, 0, GL.RGBA, GL.RGBA, GL.UNSIGNED_BYTE, png);
                } else {
                    GL.texImage2D(GL.TEXTURE_2D, 0, GL.RGB,  GL.RGB,  GL.UNSIGNED_BYTE, png);
                }
                GL.generateMipmap(GL.TEXTURE_2D);
            };
        };

        for (let text of textures) {
            const glText = GL.createTexture();
            this.mapOfTextures.set(text.name, glText);
            GL.bindTexture(GL.TEXTURE_2D, glText);

            GL.texImage2D(
                GL.TEXTURE_2D, 
                0, 
                GL.RGBA, 
                1, 
                1, 
                0, 
                GL.RGBA, 
                GL.UNSIGNED_BYTE, 
                new Uint8Array([0, 0, 255, 255])
            );

            const png  = new Image(); 
            png.onload = onLoad(text, png);
            png.src    = `/textures/${text.name}.png`;

            //image.crossOrigin = 'anonymous';
        }
    }

    initMaterials (materials) {
        for (let mat of materials) {
            this.mapOfMaterials.set(mat.name, mat);
            mat.textures.forEach((tex, i) => {
                if (tex) {
                    mat.textures[i] = this.mapOfTextures.get(tex); 
                }
            });
            mat.shader = this.mapOfShaders.get(mat.shaderProgram);
        }
    }

    initSceneGraph(node, parent = null) {
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
            refFrame = new Model(parent, node, 
                this.mapOfMaterials,
                this.mapOfShaders 
            );
            if (refFrame.isTheFloor) {
                this.mirrorObj = refFrame; 
            }
            break;
        default:
            break;
        }

        if (node.hasOwnProperty('children')) {
            for (let child of node.children) {
                this.initSceneGraph(child, refFrame);
            }
        }

        if (!parent && refFrame) {
            this.rootNode = refFrame;
        }
    }

    loadScene(url) {
        this.mapOfTextures.clear();
        this.mapOfMaterials.clear();

        if (GL) {
            const request = new XMLHttpRequest();
            request.overrideMimeType("application/json");
            request.onreadystatechange = () => {
                if (request.readyState === 4 && request.status === 200) {
                    const scene = JSON.parse(request.responseText);
                    this.initTextures(scene.textures);
                    this.initMaterials(scene.materials);
                    this.initSceneGraph(scene.sceneRoot);
                }
            };
            request.open('GET', url, true);
            request.send();                    
        }
    }

    cacheTranslucentPiece(piece) {
        this.translucentPieces.push(piece);
    }

    drawTranslucentPieces() {
        this.translucentPieces.forEach(({model, piece}) => {
            piece.material.shader.drawTriangles(model, piece);
        });
        this.translucentPieces.length = 0;
    }

    drawNode(node, mode) {
        if (node) {
            if (node instanceof Model && node !== this.mirrorObj) {
                switch (mode) {
                case DRAW.MIRROR:
                    this.drawWirefrm ? node.drawEdges() : node.drawPieces(1, this.cacheTranslucentPiece);
                    break;

                case DRAW.PIECES:
                    this.drawWirefrm ? node.drawEdges() : node.drawPieces(0, this.cacheTranslucentPiece);
                    if (this.drawNormals) {
                        node.drawNormals();
                    }
                    break;

                default:
                    break;
                }
            }

            for (let child of node.children()) {
                this.drawNode(child, mode); // recurse
            }
        }
    }

    drawScene() {
        GL.clear(GL.COLOR_BUFFER_BIT | GL.DEPTH_BUFFER_BIT | GL.STENCIL_BUFFER_BIT);
        if (this.mirrorObj) {
            GL.disable(GL.BLEND); // disable alpha blending

            GL.enable(GL.STENCIL_TEST);    // enable stencil buffer
            GL.stencilFunc(GL.ALWAYS,1,1); // stencil test always passes
            GL.stencilOp(GL.KEEP,          // if stencil test fail do nothing
                              GL.INCR,          // if stencil test pass depth test fail write 1 to stencil
                              GL.INCR);         // if stencil test pass depth test pass write 1 to stencil
            GL.depthMask(false);                      // disable depth buffer writes
            GL.colorMask(false, false, false, false); // disable color buffer writes
            
            this.mirrorObj.drawPieces(1);      // draw mirror into stencil

            GL.cullFace(GL.FRONT);         // cull CW triangles
            GL.stencilFunc(GL.EQUAL,1,1);  // stencil test pass if stencil == 1
            GL.stencilOp(GL.KEEP,          // if stencil test fail do nothing
                              GL.KEEP,          // if stencil test pass depth test fail do nothing
                              GL.KEEP);         // if stencil test pass depth test pass do nothing
            GL.depthMask(true);                   // enable depth buffer writes
            GL.colorMask(true, true, true, true); // enable color buffer writes

            // generate reflection matrix using mirror model matrix and scaling by -1 in z-axis 
            let m = new Matrix4x4(this.activeCamera.modelMatrix); 
            m = m.mul(this.mirrorObj.modelMatrix.inverse()); // world space to mirror space
            m = m.postCatSxyz(1, 1, -1);                     // to mirror reflection space
            m = m.mul(this.mirrorObj.modelMatrix);           // to world space xform

            const savedCamera = this.activeCamera; // restore this after rendering reflection
            this.activeCamera = this.mirrorCam;    // make mirror camera rendering camera
            this.activeCamera.modelMatrix = m;     // apply mirror xform to mirror camera

            this.drawNode(this.rootNode, DRAW.MIRROR); // draw reflection of model pieces into color buffer
            this.activeCamera = savedCamera;           // restore default camera

            GL.clear(GL.DEPTH_BUFFER_BIT);  // clear depth buffer
            GL.disable(GL.STENCIL_TEST);    // disable stencil buffer
            GL.cullFace(GL.BACK);           // cull CCW triangles
            GL.enable(GL.BLEND);            // enable alpha blending

            // draw mirror into color buffer
            this.drawWirefrm ? this.mirrorObj.drawEdges() : this.mirrorObj.drawPieces(1);
        }

        this.drawNode(this.rootNode, DRAW.PIECES); // draw all models in scene graph
        this.drawTranslucentPieces()

        requestAnimationFrame(this.drawScene);
    }
}
