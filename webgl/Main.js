let g_GL = null; // GL rendering context:

// In this demo model, view and world space all use a right-handed coordinate system such that 
// +ve x points right, +ve y points into the screen and +ve z points up.

function main() {
    g_GL = document.getElementById('webgl').getContext("webgl");
    if (g_GL) {
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
                    case 'Model':
                        refFrame = new Model(parent, node); 
                        break;
                }

                if (node.hasOwnProperty('children')) {
                    for (child of node.children) {
                        initSceneGraph(child, refFrame);
                    }
                }

                if (!parent) {
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
            function drawModels(node) {
                if (node) {
                    if (node instanceof Model) {
                        node.draw(true);
                    }
                    for (let child of node.children()) {
                        drawModels(child);
                    }
                }
            }

            g_GL.clear(g_GL.COLOR_BUFFER_BIT | g_GL.DEPTH_BUFFER_BIT);
            drawModels(g_GL.rootNode);

            requestAnimationFrame(g_GL.drawScene);
        };

        Object.defineProperties(g_GL, {
            activeCamera: {
                get: function() { 
                    return g_GL.cameras[g_GL.cameraIdx];
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

        g_GL.cameras   = [];
        g_GL.cameraIdx = 0;
        g_GL.mirrorCam = null;
        g_GL.mirrorObj = null;
        g_GL.ambientLS = new AmbientLS([0.25, 0.25, 0.25], [0.25, 0.25, 0.25]);
        g_GL.omniDirLS = null;
        g_GL.drawWirefrm = false;
        g_GL.drawNormals = false;

        g_GL.mapOfShaders = new Map([
            ['P3C4',   new ShaderP3C4()], 
            ['P3N3',   new ShaderP3N3()], 
            ['P3N3T2', new ShaderP3N3T2()]
        ]);

        g_GL.enable(g_GL.DEPTH_TEST);
        g_GL.enable(g_GL.BLEND);
        g_GL.blendFunc(g_GL.SRC_ALPHA, g_GL.ONE_MINUS_SRC_ALPHA);

        g_GL.clearStencil(1.0);
        g_GL.clearColor(0.392156899, 0.584313750, 0.929411829, 1.0);
        g_GL.clearDepth(1.0);

        g_GL.loadScene('http://localhost:8888/json/goku.json');
        g_GL.drawScene();

    } else {
        console.log('Failed to get rendering context for WebGL');
    }
}

