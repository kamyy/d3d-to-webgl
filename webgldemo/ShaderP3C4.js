class ShaderP3C4 extends Shader { 
    constructor(gl) {
        super(gl, 'http://localhost:8888/glsl/P3C4.vs', 'http://localhost:8888/glsl/P3C4.fs');
    }

    drawPrimitives(subModel) {
        if (this.useShaderProgramOn(subModel)) {
            const a_pos = this.gl.getAttribLocation(this.program, 'a_pos');
            const a_col = this.gl.getAttribLocation(this.program, 'a_col'); 

            if (a_pos !== -1 && a_col !== -1) {
                this.gl.bindBuffer(gl.ARRAY_BUFFER, vtxBuffer);

                this.gl.vertexAttribPointer(a_pos, 3, this.gl.FLOAT, false, 28, 0 );
                this.gl.enableVertexAttribArray(a_pos);
                this.gl.vertexAttribPointer(a_col, 4, this.gl.FLOAT, false, 28, 12);
                this.gl.enableVertexAttribArray(a_col);

                this.gl.drawArray(gl.LINES, 0, vtxBuffer.vtxCount);
            }
        }
    }

    getVertexComponentNames() {
        return ['x', 'y', 'z', 'r', 'g', 'b', 'a'];
    }
}

