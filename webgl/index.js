let g_GL = null; // GL rendering context:

function main() {
    g_GL = document.getElementById('webgl').getContext("webgl");
    if (g_GL) {

        Object.defineProperties(g_GL, { 

            initScene: {
                value: function() {
                    this.rootSpace = null;
                    this.cameras = [];
                    this.cameraIdx = 0;
                    this.mirrorCam = null;
                    this.mirrorObj = null;
                    this.ambientLS = null;
                    this.omniDirLS = null;
                    this.drawWirefrm = false;
                    this.drawNormals = false;
                    this.sceneLoaded = false;

                    new ShaderP3C4();
                    new ShaderP3N3();
                    new ShaderP3N3T2();
                }
            },

            loadScene: {
                value: function(url) {

                    const initTextures = json => {
                        this.mapOfTextures = new Map();

                        for (t of json.textures) {
                            this.mapOfTextures.set(t.name, t);

                            t.texture = g_GL.createTexture();
                            const img = new Image();

                            img.onload = function() {
                                g_GL.bindTexture(g_GL.TEXTURE_2D, t.texture);
                                if (t.hasAlpha) {
                                    g_GL.texImage2D(g_GL.TEXTURE_2D, 0, g_GL.RGBA, g_GL.RGBA, g_GL.UNSIGNED_BYTE, this);
                                } else {
                                    g_GL.texImage2D(g_GL.TEXTURE_2D, 0, g_GL.RGB,  g_GL.RGB,  g_GL.UNSIGNED_BYTE, this);
                                }
                                g_GL.generateMipmap(g_GL.TEXTURE_2D);
                            };

                            img.crossOrigin = 'anonymous';
                            img.src = 'http://localhost:8888/textures/' + t.name + '.png';
                        }
                    };

                    const initMaterials = json => {
                        this.mapOfMaterials = new Map();

                        for (m of json.materials) {
                            this.mapOfMaterials.set(m.name, m);
                        }
                    };

                    const initSceneGraph = json => {

                    };

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

        g_GL.initScene();
        g_GL.loadScene('http://localhost:8888/json/goku.json');

        g_GL.clearColor(0.0, 0.0, 0.0, 1.0);
        g_GL.clear(g_GL.COLOR_BUFFER_BIT);

    } else {
        console.log('Failed to get rendering context for WebGL');
    }
}

