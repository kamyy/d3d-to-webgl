class ShaderP3C4 extends Shader { 
    constructor() {
        super('P3C4', 'http://localhost:8888/glsl/P3C4.vs', 'http://localhost:8888/glsl/P3C4.fs');
    }

    drawPrimitives(subModel) {
        if (super.drawPrimitives(subModel)) {
            const a_pos = g_GL.getAttribLocation(this.program, 'a_pos');
            const a_col = g_GL.getAttribLocation(this.program, 'a_col'); 

            if (a_pos !== -1 && a_col !== -1) {
                g_GL.bindBuffer(gl.ARRAY_BUFFER, vtxBuffer);

                g_GL.vertexAttribPointer(a_pos, 3, g_GL.FLOAT, false, 28, 0 );
                g_GL.enableVertexAttribArray(a_pos);
                g_GL.vertexAttribPointer(a_col, 4, g_GL.FLOAT, false, 28, 12);
                g_GL.enableVertexAttribArray(a_col);

                g_GL.drawArray(g_GL.LINES, 0, vtxBuffer.vtxCount);
            }
        }
    }

    getVertexComponentNames() {
        return ['x', 'y', 'z', 'r', 'g', 'b', 'a'];
    }
}


