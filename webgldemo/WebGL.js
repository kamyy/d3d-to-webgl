function main() {
    const gl = document.getElementById('webgl').getContext("webgl");
    if (gl) {
        const shaderP3C4 = new ShaderP3C4(gl);

        gl.clearColor(0.0, 0.0, 0.0, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT);
    } else {
        console.log('Failed to get rendering context for WebGL');
    }
}

