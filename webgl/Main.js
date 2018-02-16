// Model, view and world space all use a right-handed coordinate system such that 
// +ve x points right, +ve y points into the screen and +ve z points up.

let g_GL = null; // GL rendering context:

function main() {
    const canvas = document.getElementById('webgl');

    g_GL = canvas.getContext("webgl", {
        depth: true,
        alpha: false,
        stencil: true
    });

    if (g_GL) {
        let buttonDown = false;
        let lx = 0;
        let ly = 0;
        
        canvas.onmousedown = function(event) {
            const rect = canvas.getBoundingClientRect();
            const x = event.clientX;
            const y = event.clientY;

            if (event.button === 0 && x > rect.left && x < rect.right && y > rect.top && y < rect.bottom) {
                buttonDown = true; 
                lx = x; 
                ly = y;
            }
        };

        window.onmousemove = function(event) {
            const TXYZ_SCALAR = 0.01;
            const RXYZ_SCALAR = 0.25;
            const x = event.clientX;
            const y = event.clientY;

            if (buttonDown && (x != lx || y != ly)) {
                const camera = g_GL.activeCamera;
                const target = g_GL.activeCamera.parent;

                if (event.altKey) {
                    target.rotateZ(degreesToRadians(lx - x) * RXYZ_SCALAR); // yaw camera target around it's own z-axis
                    camera.rotateX(degreesToRadians(ly - y) * RXYZ_SCALAR, target); // pitch around camera target's x-axis

                } else if (event.shiftKey) {
                    camera.translate(new Vector1x4(0, (x - lx) * TXYZ_SCALAR, 0));
                    
                } else {
                    const dx = (lx - x) * TXYZ_SCALAR;
                    const dz = (y - ly) * TXYZ_SCALAR;
                    const dv = camera.mapPos(new Vector1x4(dx, 0, dz, 0), target);
                    target.translate(dv) // move target along own axes

                }
                lx = x;
                ly = y;
            }
        };

        window.onmouseup = function(event) {
            if (event.button === 0) {
                buttonDown = false;
            }
        };

        const DRAW = Object.freeze({
            NORMAL: Symbol("normal"),
            MIRROR: Symbol("mirror"),
            PIECES: Symbol("pieces")
        });

        g_GL.loadScene = function(url) {
            function initTextures(textures) {
                g_GL.mapOfTextures = new Map();

                for (let t of textures) {
                    g_GL.mapOfTextures.set(t.name, g_GL.createTexture());
                    g_GL.bindTexture(g_GL.TEXTURE_2D, g_GL.mapOfTextures.get(t.name));
                    g_GL.texImage2D(g_GL.TEXTURE_2D, 0, g_GL.RGBA, 1, 1, 0, g_GL.RGBA, g_GL.UNSIGNED_BYTE, new Uint8Array([0, 0, 255, 255]));

                    const image = new Image();
                    image.onload = () => {
                        g_GL.bindTexture(g_GL.TEXTURE_2D, g_GL.mapOfTextures.get(t.name));
                        if (t.hasAlpha) {
                            g_GL.texImage2D(g_GL.TEXTURE_2D, 0, g_GL.RGBA, g_GL.RGBA, g_GL.UNSIGNED_BYTE, image);
                        } else {
                            g_GL.texImage2D(g_GL.TEXTURE_2D, 0, g_GL.RGB,  g_GL.RGB,  g_GL.UNSIGNED_BYTE, image);
                        }
                        g_GL.generateMipmap(g_GL.TEXTURE_2D);
                    };

                    image.crossOrigin = 'anonymous';
                    image.src = 'http://localhost:8888/textures/' + t.name + '.png';
                }
            }

            function initMaterials(materials) {
                g_GL.mapOfMaterials = new Map();

                for (let m of materials) {
                    g_GL.mapOfMaterials.set(m.name, m);
                    m.textures.forEach((t, i) => {
                        if (t) {
                            m.textures[i] = g_GL.mapOfTextures.get(t); 
                        }
                    });
                    m.shader = g_GL.mapOfShaders.get(m.shaderProgram);
                }
            }

            function initSceneGraph(node, parent) {
                let refFrame = null;

                switch(node.nodeType) {
                    case 'OmniDirLS':
                        refFrame = new OmniDirLS(parent, node); 
                        g_GL.omniDirLS = refFrame;
                        break;
                    case 'RefFrame':
                        refFrame = new RefFrame(parent, node); 
                        break;
                    case 'Camera':
                        refFrame = new Camera(parent, node);
                        g_GL.cameras.push(refFrame); 
                        break;
                    case 'MirrorCamera':
                        refFrame = new Camera(parent, node);
                        g_GL.mirrorCam = refFrame; 
                        break;
                    case 'Model':
                        refFrame = new Model(parent, node); 
                        if (refFrame.isFloor()) {
                           g_GL.mirrorObj = refFrame; 
                        }
                        break;
                }

                if (node.hasOwnProperty('children')) {
                    for (child of node.children) {
                        initSceneGraph(child, refFrame);
                    }
                }

                if (!parent && refFrame) {
                    g_GL.rootNode = refFrame;
                }
            }

            const request = new XMLHttpRequest();
            request.overrideMimeType("application/json");
            request.onreadystatechange = () => {
                if (request.readyState === 4 && request.status == 200) {
                    const scene = JSON.parse(request.responseText);
                    initTextures(scene.textures);
                    initMaterials(scene.materials);
                    initSceneGraph(scene.sceneRoot);
                }
            }
            request.open('GET', url, true);
            request.send();                    
        };

        g_GL.drawScene = function() {
            function draw(node, type) {
                if (node) {
                    if (node instanceof Model) {
                        switch (type) {
                            case DRAW.NORMAL:
                                node.drawNormals(); // draw model pieces in scene graph for normals in vertices
                                break;

                            case DRAW.MIRROR:
                                if (node !== g_GL.mirrorObj) {
                                    node.drawPieces(1); // draw model pieces in scene graph for reflection
                                }
                                break;

                            case DRAW.PIECES:
                                if (node !== g_GL.mirrorObj) {
                                    node.drawPieces(0); // draw model pieces in scene graph
                                }
                                break;
                        }
                    }

                    for (let child of node.children()) {
                        draw(child, type); // recurse
                    }

                    if (node === g_GL.rootNode && type === DRAW.PIECES) {
                        for (let obj of g_GL.alphaPieces) { // draw translucent pieces
                            let model = obj.model;
                            let piece = obj.piece;
                            piece.material.shader.drawTriangles(model, piece);
                        }
                        g_GL.alphaPieces.length = 0;
                    }
                }
            }

            g_GL.clear(g_GL.COLOR_BUFFER_BIT | g_GL.DEPTH_BUFFER_BIT | g_GL.STENCIL_BUFFER_BIT);
            if (g_GL.mirrorObj) {
                g_GL.disable(g_GL.BLEND); // disable alpha blending

                g_GL.enable(g_GL.STENCIL_TEST);    // enable stencil buffer
                g_GL.stencilFunc(g_GL.ALWAYS,1,1); // stencil test always passes
                g_GL.stencilOp(g_GL.KEEP,          // if stencil test fail do nothing
                               g_GL.INCR,          // if stencil test pass depth test fail write 1 to stencil
                               g_GL.INCR);         // if stencil test pass depth test pass write 1 to stencil
                g_GL.depthMask(false);                      // disable depth buffer writes
                g_GL.colorMask(false, false, false, false); // disable color buffer writes
                
                g_GL.mirrorObj.drawPieces(1);      // draw mirror into stencil

                g_GL.cullFace(g_GL.FRONT);         // cull CW triangles
                g_GL.stencilFunc(g_GL.EQUAL,1,1);  // stencil test pass if stencil == 1
                g_GL.stencilOp(g_GL.KEEP,          // if stencil test fail do nothing
                               g_GL.KEEP,          // if stencil test pass depth test fail do nothing
                               g_GL.KEEP);         // if stencil test pass depth test pass do nothing
                g_GL.depthMask(true);                   // enable depth buffer writes
                g_GL.colorMask(true, true, true, true); // enable color buffer writes

                // generate reflection matrix using mirror model matrix and scaling by -1 in z-axis 
                let m = new Matrix4x4(g_GL.activeCamera.modelMatrix); 
                m = m.mul(g_GL.mirrorObj.modelMatrix.inverse()); // world space to mirror space
                m = m.postCatSxyz(1, 1, -1);                     // to mirror reflection space
                m = m.mul(g_GL.mirrorObj.modelMatrix);           // to world space xform

                const savedCamera = g_GL.activeCamera; // restore this after rendering reflection
                g_GL.activeCamera = g_GL.mirrorCam;    // make mirror camera rendering camera
                g_GL.activeCamera.modelMatrix = m;     // apply mirror xform to mirror camera

                draw(g_GL.rootNode, DRAW.MIRROR);   // draw reflection of model pieces into color buffer
                
                g_GL.clear(g_GL.DEPTH_BUFFER_BIT);  // clear depth buffer
                g_GL.activeCamera = savedCamera;    // restore default camera
                g_GL.disable(g_GL.STENCIL_TEST);    // disable stencil buffer
                g_GL.cullFace(g_GL.BACK);           // cull CCW triangles
                g_GL.enable(g_GL.BLEND);            // enable alpha blending

                g_GL.mirrorObj.drawPieces(1);       // draw mirror into color buffer
            }
            draw(g_GL.rootNode, DRAW.PIECES);

            requestAnimationFrame(g_GL.drawScene);
        };

        Object.defineProperties(g_GL, {
            activeCamera: {
                get: function() { 
                    return g_GL.cameras[g_GL.cameraIdx];
                },
                set: function(cam) {
                    g_GL.cameras[g_GL.cameraIdx] = cam;
                }
            },
            activeCamIdx: {
                get: function() { 
                    return g_GL.cameraIdx;
                },
                set: function(idx) { 
                    g_GL.cameraIdx = idx;
                }
            }
        });

        g_GL.cameras = [];
        g_GL.cameraIdx = 0;
        g_GL.mirrorCam = null;
        g_GL.mirrorObj = null;
        g_GL.ambientLS = new AmbientLS([0.15, 0.15, 0.15], [0.15, 0.15, 0.15]);
        g_GL.omniDirLS = null;
        g_GL.alphaPieces = [];
        g_GL.drawWirefrm = false;
        g_GL.drawNormals = false;

        g_GL.mapOfShaders = Object.freeze(new Map([
            ['P3C3',     new ShaderP3C3()], 
            ['P3N3',     new ShaderP3N3()], 
            ['P3N3T2',   new ShaderP3N3T2()],
            ['P3N3B3T2', new ShaderP3N3B3T2()]
        ]));

        g_GL.depthFunc(g_GL.LESS);    // less than depth test
        g_GL.enable(g_GL.DEPTH_TEST); // enable depth testing
        g_GL.enable(g_GL.CULL_FACE);  // enable backface culling
        g_GL.enable(g_GL.BLEND);      // enable alpha blending
        g_GL.blendFunc(g_GL.SRC_ALPHA, g_GL.ONE_MINUS_SRC_ALPHA);

        g_GL.clearColor(0.392156899, 0.584313750, 0.929411829, 1.0); // cornflower blue
        g_GL.clearDepth(1.0); // depth buffer clear value
        g_GL.clearStencil(0); // stencil buffer clear value 

        g_GL.loadScene('http://localhost:8888/json/hardwood.json');
        g_GL.drawScene();

    } else {
        console.log('Failed to get rendering context for WebGL');
    }
}

