let g_GL = null; // GL context

const g_webGLdemo = new class {
    constructor() {
        this.worldSpace = new RefFrame();
        this.cameras = [];
        this.activeCamIdx = 0;
        this.mirrorCamera = null;
        this.ambientLS = null;
        this.omniDirLS = null;
        this.actors = [];
        this.mirror = null;
        this.drawWirefrm = false;
        this.drawNormals = false;
    }

    get activeCamera() {
        return this.cameras[this.activeCamIdx]; 
    }

    init() {
        const shaderP3C4 = new ShaderP3C4();
        const shaerrP3N3 = new ShaderP3N3();
    }

    loadScene() {

    }
};

function main() {
    g_GL = document.getElementById('webgl').getContext("webgl");
    if (g_GL) {
        g_GL.clearColor(0.0, 0.0, 0.0, 1.0);
        g_GL.clear(g_GL.COLOR_BUFFER_BIT);
    } else {
        console.log('Failed to get rendering context for WebGL');
    }
}

