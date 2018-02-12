let g_GL = null; // GL rendering context:

function main() {
    g_GL = document.getElementById('webgl').getContext("webgl");
    if (g_GL) {

        Object.defineProperties(g_GL, { 
            loadScene: {
                value: function(url) {
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

                    const initTextures = (json) => {
                        for (let t of json.textures) {
                            this.mapOfTextures.set(t.name, t);

                            t.texture = this.createTexture();
                            const img = new Image();

                            img.onload = () => {
                                this.bindTexture(this.TEXTURE_2D, t.texture);
                                if (t.hasAlpha) {
                                    this.texImage2D(this.TEXTURE_2D, 0, this.RGBA, this.RGBA, this.UNSIGNED_BYTE, img);
                                } else {
                                    this.texImage2D(this.TEXTURE_2D, 0, this.RGB,  this.RGB,  this.UNSIGNED_BYTE, img);
                                }
                                this.generateMipmap(this.TEXTURE_2D);
                            };

                            img.crossOrigin = 'anonymous';
                            img.src = 'http://localhost:8888/textures/' + t.name + '.png';
                        }
                    };

                    const initMaterials = (json) => {
                        for (let m of json.materials) {
                            this.mapOfMaterials.set(m.name, m);
                            m.textures.forEach((name, idx) => {
                                if (name) {
                                    m.textures[idx] = this.mapOfTextures.get(name); 
                                }
                            });
                            m.shaderProgram = this
                        }
                    };

                    const initSceneGraph = (json) => {

                    };
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

