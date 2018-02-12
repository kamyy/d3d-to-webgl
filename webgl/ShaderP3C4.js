class ShaderP3C4 extends Shader { 
    constructor() {
        super('http://localhost:8888/glsl/P3C4.vert', 'http://localhost:8888/glsl/P3C4.frag');
    }

    drawNormals(model, modelPiece) {
        if (this.program) {
            g_GL.useProgram(this.program);
            this.setUniformVariablesInVertShader(model);
            this.setUniformVariablesInFragShader(model);

            g_GL.bindBuffer(g_GL.ARRAY_BUFFER, modelPiece.nrmBuffer);

            for (let desc of this.vertexAttributeDescs) {
                const loc = g_GL.getAttribLocation(this.program, desc.attrib);
                if (loc !== -1) {
                    g_GL.vertexAttribPointer(loc, desc.length, g_GL.FLOAT, false, desc.stride, desc.offset);
                    g_GL.enableVertexAttribArray(loc);
                }
            }

            g_GL.drawArray(g_GL.LINES, 0, modelPiece.nrmVtxCount);
        }
    }

    get vertexAttributeDescs() {
        return ShaderP3C4.vertexAttributeDescs;
    }
}

ShaderP3C4.vertexAttributeDescs = [ 
    {   attrib: 'a_pos',
        length: 3,
        stride: 28,
        offset: 0
    },
    {   attrib: 'a_col',
        length: 4,
        stride: 28,
        offset: 12
    }
];
