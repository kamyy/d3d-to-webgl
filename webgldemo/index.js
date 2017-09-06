function main() {
    const gl = document.getElementById('index').getContext("webgl");
    if (gl) {
        gl.clearColor(0.0, 0.0, 0.0, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT);
    } else {
        console.log('Failed to get rendering context for WebGL');
    }
}
