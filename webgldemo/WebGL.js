function main() {
    const gl = document.getElementById('webgl').getContext("webgl");
    if (gl) {
        const shader = new Shader(
            gl, 
            'http://localhost:8888/P3C4.vs.glsl',
            'http://localhost:8888/P3C4.fs.glsl'
        );
        gl.clearColor(0.0, 0.0, 0.0, 1.0);
        gl.clear(gl.COLOR_BUFFER_BIT);
    } else {
        console.log('Failed to get rendering context for WebGL');
    }
}

