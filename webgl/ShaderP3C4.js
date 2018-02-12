class ShaderP3C4 extends Shader { 
    constructor() {
        super('http://localhost:8888/glsl/P3C4.vert', 'http://localhost:8888/glsl/P3C4.frag');
    }

    drawNormals(modelPiece) {
        if (this.program) {
            const { model, nrmBuffer } = modelPiece;

            g_GL.useProgram(this.program);
            this.setUniformModelSpaceUpDir(model);

            g_GL.bindBuffer(g_GL.ARRAY_BUFFER, nrmBuffer);

            for (desc of this.vertexAttributeDescs) {
                const location = g_GL.getAttribLocation(this.program, desc.attrib);
                if (location !== -1) {
                    g_GL.vertexAttribPointer(location, desc.length, g_GL.FLOAT, false, desc.stride, desc.offset);
                    g_GL.enableVertexAttribArray(location);
                }
            }

            g_GL.drawArray(g_GL.LINES, 0, modelPiece.nrms.length);
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
