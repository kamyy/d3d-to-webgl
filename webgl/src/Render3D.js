import Model from './Model';
import Vector1x4 from './Vector1x4';
import Matrix4x4 from './Matrix4x4';
import ShaderP3C3 from './ShaderP3C3';
import ShaderP3N3 from './ShaderP3N3';
import ShaderP3N3T2 from './ShaderP3N3T2';
import ShaderP3N3B3T2 from './ShaderP3N3B3T2';

import { g_scene } from './App';

import React, { Component } from 'react';

const DRAW = Object.freeze({
    MIRROR: Symbol("mirror"),
    PIECES: Symbol("pieces")
});

export let g_GL = null;

// Model, view and world space all use a right-handed coordinate system such that 
// +ve x points right, +ve y points into the screen and +ve z points up.

export default class Render3D extends Component {
    constructor(props) {
        super(props);

        this.TXYZ_SCALAR = 0.01;
        this.RXYZ_SCALAR = 0.25;
        this.buttonDown = false;
        this.lx = 0;
        this.ly = 0;

        this.onMouseDown = this.onMouseDown.bind(this);
        this.onMouseMove = this.onMouseMove.bind(this);
        this.onMouseUp = this.onMouseUp.bind(this);
        this.drawScene = this.drawScene.bind(this);
    }

    render() {
        return <canvas id="canvas" width="1280" height="720">Please use a browser that supports WebGL</canvas>
    }

    componentDidMount() {
        this.canvas = document.getElementById('canvas');
        g_GL = this.canvas.getContext("webgl", {
            depth: true,
            alpha: false,
            stencil: true
        });

        if (g_GL) {
            g_scene.mapOfShaders = Object.freeze(new Map([
                ['P3C3',     new ShaderP3C3()], 
                ['P3N3',     new ShaderP3N3()], 
                ['P3N3T2',   new ShaderP3N3T2()],
                ['P3N3B3T2', new ShaderP3N3B3T2()]
            ]));

            this.canvas.onmousedown = this.onMouseDown;
            window.onmousemove = this.onMouseMove;
            window.onmouseup = this.onMouseUp;

            g_GL.depthFunc(g_GL.LESS);    // less than depth test
            g_GL.enable(g_GL.DEPTH_TEST); // enable depth testing
            g_GL.enable(g_GL.CULL_FACE);  // enable backface culling
            g_GL.enable(g_GL.BLEND);      // enable alpha blending
            g_GL.blendFunc(g_GL.SRC_ALPHA, g_GL.ONE_MINUS_SRC_ALPHA);

            g_GL.clearColor(0.392156899, 0.584313750, 0.929411829, 1.0); // cornflower blue
            g_GL.clearDepth(1.0); // depth buffer clear value
            g_GL.clearStencil(0); // stencil buffer clear value 
            
            g_scene.loadScene('/json/hardwood.json');
            this.drawScene();
        }
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
            const camera = g_scene.activeCamera;
            const target = g_scene.activeCamera.parent;

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

    drawNode(node, mode) {
        if (node) {
            if (node instanceof Model && node !== g_scene.mirrorObj) {
                switch (mode) {
                case DRAW.MIRROR:
                    g_scene.drawWirefrm ? node.drawEdges() : node.drawPieces(1);
                    break;

                case DRAW.PIECES:
                    g_scene.drawWirefrm ? node.drawEdges() : node.drawPieces(0);
                    if (g_scene.drawNormals) {
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
        g_GL.clear(g_GL.COLOR_BUFFER_BIT | g_GL.DEPTH_BUFFER_BIT | g_GL.STENCIL_BUFFER_BIT);
        if (g_scene.mirrorObj) {
            g_GL.disable(g_GL.BLEND); // disable alpha blending

            g_GL.enable(g_GL.STENCIL_TEST);    // enable stencil buffer
            g_GL.stencilFunc(g_GL.ALWAYS,1,1); // stencil test always passes
            g_GL.stencilOp(g_GL.KEEP,          // if stencil test fail do nothing
                        g_GL.INCR,          // if stencil test pass depth test fail write 1 to stencil
                        g_GL.INCR);         // if stencil test pass depth test pass write 1 to stencil
            g_GL.depthMask(false);                      // disable depth buffer writes
            g_GL.colorMask(false, false, false, false); // disable color buffer writes
            
            g_scene.mirrorObj.drawPieces(1);      // draw mirror into stencil

            g_GL.cullFace(g_GL.FRONT);         // cull CW triangles
            g_GL.stencilFunc(g_GL.EQUAL,1,1);  // stencil test pass if stencil == 1
            g_GL.stencilOp(g_GL.KEEP,          // if stencil test fail do nothing
                        g_GL.KEEP,          // if stencil test pass depth test fail do nothing
                        g_GL.KEEP);         // if stencil test pass depth test pass do nothing
            g_GL.depthMask(true);                   // enable depth buffer writes
            g_GL.colorMask(true, true, true, true); // enable color buffer writes

            // generate reflection matrix using mirror model matrix and scaling by -1 in z-axis 
            let m = new Matrix4x4(g_scene.activeCamera.modelMatrix); 
            m = m.mul(g_scene.mirrorObj.modelMatrix.inverse()); // world space to mirror space
            m = m.postCatSxyz(1, 1, -1);                         // to mirror reflection space
            m = m.mul(g_scene.mirrorObj.modelMatrix);           // to world space xform

            const savedCamera = g_scene.activeCamera; // restore this after rendering reflection
            g_scene.activeCamera = g_scene.mirrorCam; // make mirror camera rendering camera
            g_scene.activeCamera.modelMatrix = m;     // apply mirror xform to mirror camera

            this.drawNode(g_scene.rootNode, DRAW.MIRROR); // draw reflection of model pieces into color buffer
            g_scene.activeCamera = savedCamera; // restore default camera

            g_GL.clear(g_GL.DEPTH_BUFFER_BIT);  // clear depth buffer
            g_GL.disable(g_GL.STENCIL_TEST);    // disable stencil buffer
            g_GL.cullFace(g_GL.BACK);           // cull CCW triangles
            g_GL.enable(g_GL.BLEND);            // enable alpha blending

            // draw mirror into color buffer
            g_scene.drawWirefrm ? g_scene.mirrorObj.drawEdges() : g_scene.mirrorObj.drawPieces(1);
        }

        this.drawNode(g_scene.rootNode, DRAW.PIECES); // draw all models in scene graph
        g_scene.alphaPieces.forEach( ({model, piece}) =>  
            piece.material.shader.drawTriangles(model, piece)
        );
        g_scene.alphaPieces.length = 0;

        requestAnimationFrame(this.drawScene);
    }

}
