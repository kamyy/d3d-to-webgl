const g_worldUp = new Vector1x4(0.0, 0.0, 1.0, 0.0); // up direction 
const g_origin  = new Vector1x4(0.0, 0.0, 0.0, 1.0); // origin
let g_GL = null; // GL context

function main() {
    g_GL = document.getElementById('webgl').getContext("webgl");
    if (g_GL) {
        const shaderP3C4 = new ShaderP3C4();

        g_GL.clearColor(0.0, 0.0, 0.0, 1.0);
        g_GL.clear(gl.COLOR_BUFFER_BIT);
    } else {
        console.log('Failed to get rendering context for WebGL');
    }
}

