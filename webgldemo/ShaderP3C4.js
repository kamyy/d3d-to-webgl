class ShaderP3C4 extends Shader { 
    constructor() {
        super('P3C4', 'http://localhost:8888/glsl/P3C4.vert', 'http://localhost:8888/glsl/P3C4.frag');
    }

    drawPrimitives(subModel) {
        if (this.program) {
            g_GL.useProgram(this.program);
            //
            // model view projection matrix
            // model space camera pos
            // model space omniLS pos
            this.setModelSpaceUpDir(subModel.model);

            g_GL.bindBuffer(g_GL.ARRAY_BUFFER, subModel.nrmBuffer);

            for (d of this.vertexAttributeDescs) {
                const location = g_GL.getAttribLocation(this.program, d.attrib);
                if (location !== -1) {
                    g_GL.vertexAttribPointer(location, d.length, g_GL.FLOAT, false, d.stride, d.offset);
                    g_GL.enableVertexAttribArray(location);
                }
            }

            g_GL.drawArray(g_GL.LINES, 0, subModel.nrmBuffer.vtxCount);
        }
    }

    get vertexComponentNames() {
        return ShaderP3C4.vertexComponentNames;
    }

    get vertexAttributeDescs() {
        return ShaderP3C4.vertexAttributeDescs;
    }
}

ShaderP3C4.vertexComponentNames = [
    'x', 'y', 'z', 'r', 'g', 'b', 'a'
];

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
