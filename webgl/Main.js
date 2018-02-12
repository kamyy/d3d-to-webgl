let g_GL = null; // GL rendering context:

function main() {
    g_GL = document.getElementById('webgl').getContext("webgl");
    if (g_GL) {

        Object.defineProperties(g_GL, { 
            loadScene: {
                value: function(url) {

                    function initTextures(json) {
                        for (let texture of json.textures) {
                            g_GL.mapOfTextures.set(texture.name, g_GL.createTexture());

                            const image = new Image();
                            image.onload = () => {
                                g_GL.bindTexture(g_GL.TEXTURE_2D, g_GL.mapOfTextures.get(texture.name));
                                if (texture.hasAlpha) {
                                    g_GL.texImage2D(g_GL.TEXTURE_2D, 0, g_GL.RGBA, g_GL.RGBA, g_GL.UNSIGNED_BYTE, image);
                                } else {
                                    g_GL.texImage2D(g_GL.TEXTURE_2D, 0, g_GL.RGB,  g_GL.RGB,  g_GL.UNSIGNED_BYTE, image);
                                }
                                g_GL.generateMipmap(g_GL.TEXTURE_2D);
                            };

                            image.crossOrigin = 'anonymous';
                            image.src = 'http://localhost:8888/textures/' + texture.name + '.png';
                        }
                    }

                    function initMaterials(json) {
                        for (let m of json.materials) {
                            g_GL.mapOfMaterials.set(m.name, m);
                            m.textures.forEach((t, i) => {
                                if (t) {
                                    m.textures[i] = g_GL.mapOfTextures.get(t); 
                                }
                            });
                            m.shader = g_GL.mapOfShaders.get(m.shaderProgram);
                        }
                    }

                    function initSceneGraph(json) {

                    }

                    this.rootSpace = new RefFrame();
                    this.cameras = [];
                    this.cameraIdx = 0;
                    this.mirrorCam = null;
                    this.mirrorObj = null;
                    this.ambientLS = new AmbientLS([0.25, 0.25, 0.25], [0.25, 0.25, 0.25]);
                    this.omniDirLS = null;
                    this.drawWirefrm = false;
                    this.drawNormals = false;
                    this.sceneLoaded = false;

                    this.mapOfShaders = new Map();
                    this.mapOfTextures = new Map();
                    this.mapOfMaterials = new Map();

                    this.mapOfShaders.set('P3C4', new ShaderP3C4());
                    this.mapOfShaders.set('P3N3', new ShaderP3N3());
                    this.mapOfShaders.set('P3N3T2', new ShaderP3N3T2());

                    const request = new XMLHttpRequest();
                    request.overrideMimeType("application/json");
                    request.onreadystatechange = () => {
                        if (request.readyState === 4 && request.status == 200) {
                            const json = JSON.parse(request.responseText);
                            initTextures(json);
                            initMaterials(json);
                            initSceneGraph(json);
                        }
                    }
                    request.open('GET', url, true);
                    request.send();                    
                }
            },

            activeCamera: {
                get: function() { 
                    return this.cameras[this.cameraIdx];
                } 
            },

            activeCameraIdx: {
                get: function() { 
                    return this.cameraIdx;
                },
                set: function(idx) { 
                    this.cameraIdx = idx;
                }
            }
        });

        g_GL.loadScene('http://localhost:8888/json/goku.json');

        g_GL.clearColor(0.0, 0.0, 0.0, 1.0);
        g_GL.clear(g_GL.COLOR_BUFFER_BIT);

    } else {
        console.log('Failed to get rendering context for WebGL');
    }
}

