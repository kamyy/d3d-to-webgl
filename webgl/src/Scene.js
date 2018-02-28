import Model from './Model';
import Camera from './Camera';
import RefFrame from './RefFrame';
import AmbientLS from './AmbientLS';
import OmniDirLS from './OmniDirLS';
import Matrix4x4 from './Matrix4x4';

import { GL } from './App'

const DRAW = Object.freeze({
    MIRROR: Symbol("mirror"),
    PIECES: Symbol("pieces")
});

export default class Scene {
    constructor(name, getScene) {
        this.name = name;
        this.cameras = [];
        this.rootNode = null;
        this.cameraIdx = 0;
        this.mirrorCam = null;
        this.mirrorObj = null;
        this.ambientLS = new AmbientLS([0.15, 0.15, 0.15], [0.15, 0.15, 0.15]);
        this.omniDirLS = null;

        this.drawWirefrm = false;
        this.drawNormals = false;

        this.mapOfTextures = new Map();
        this.mapOfMaterials = new Map();
        this.translucentPieces = [];

        this.getScene = getScene;

        this.drawScene = this.drawScene.bind(this);
        this.cacheTranslucentPiece = this.cacheTranslucentPiece.bind(this);

        this.onChangeCameraSelect = this.onChangeCameraSelect.bind(this);
        this.onChangeDrawNormalsCheckbox = this.onChangeDrawNormalsCheckbox.bind(this);
        this.onChangeDrawWirefrmCheckbox = this.onChangeDrawWirefrmCheckbox.bind(this);

        this.requestedLoad = false;
        this.onSceneLoaded = null;
    }

    loadScene() {
        if (GL && !this.requestedLoad) {
            this.requestedLoad = true;

            const request = new XMLHttpRequest();
            request.overrideMimeType("application/json");
            request.onreadystatechange = () => {
                if (request.readyState === 4 && request.status === 200) {
                    const scene = JSON.parse(request.responseText);

                    this.initTextures(scene.textures);
                    this.initMaterials(scene.materials);
                    this.initSceneGraph(scene.sceneRoot);

                    if (this.onSceneLoaded) {
                        this.onSceneLoaded();
                    }
                }
            };
            request.open('GET', `/json/${this.name}.json`, true);
            request.send();                    
        }
    }

    initTextures(textures) {
        const onLoad = (tex, png) => {
            return () => { // higher order function
                GL.bindTexture(GL.TEXTURE_2D, this.mapOfTextures.get(tex.name));
                if (tex.hasAlpha) {
                    GL.texImage2D(GL.TEXTURE_2D, 0, GL.RGBA, GL.RGBA, GL.UNSIGNED_BYTE, png);
                } else {
                    GL.texImage2D(GL.TEXTURE_2D, 0, GL.RGB,  GL.RGB,  GL.UNSIGNED_BYTE, png);
                }
                GL.generateMipmap(GL.TEXTURE_2D);
            };
        };

        for (let tex of textures) {
            const glTex = GL.createTexture();
            GL.bindTexture(GL.TEXTURE_2D, glTex);
            this.mapOfTextures.set(tex.name, glTex);

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
            png.onload = onLoad(tex, png);
            png.src    = `/textures/${tex.name}.png`;
        }
    }

    initMaterials(materials) {
        for (let mat of materials) {
            this.mapOfMaterials.set(mat.name, mat);
            mat.textures.forEach((tex, i) => {
                if (tex) {
                    mat.textures[i] = this.mapOfTextures.get(tex); 
                }
            });
            mat.shader = GL.mapOfShaders.get(mat.shaderProgram);
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
                this.mapOfMaterials
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

    drawScene() {
        if (this === this.getScene()) {
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

                // copy active camera settings to mirror camera
                this.mirrorCam.fieldOfView = this.activeCamera.fieldOfView;
                this.mirrorCam.aspectRatio = this.activeCamera.aspectRatio;
                this.mirrorCam.clipDistanceN = this.activeCamera.clipDistanceN;
                this.mirrorCam.clipDistanceF = this.activeCamera.clipDistanceF;

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
            this.drawTranslucentPieces();

            requestAnimationFrame(this.drawScene);
        }
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

    drawTranslucentPieces() {
        this.translucentPieces.forEach(({model, piece}) => {
            piece.material.shader.drawTriangles(model, piece);
        });
        this.translucentPieces.length = 0;
    }

    cacheTranslucentPiece(piece) {
        this.translucentPieces.push(piece);
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

    onChangeCameraSelect(idx) {
        this.activeCamIdx = idx;
    }

    onChangeDrawNormalsCheckbox(checked) {
        this.drawNormals = checked;
    }

    onChangeDrawWirefrmCheckbox(checked) {
        this.drawWirefrm = checked;
    }
}