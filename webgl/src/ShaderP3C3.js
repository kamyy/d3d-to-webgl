import Shader from './Shader';
import { GL } from './App';

export default class ShaderP3C3 extends Shader { 
    constructor(getScene) {
        super('/glsl/P3C3.vert', '/glsl/P3C3.frag', getScene);
    }

    drawNormals(model, modelPiece) {
        if (this.program) {
            GL.useProgram(this.program);
            this.setUniformVariablesInVertShader(model);
            this.setUniformVariablesInFragShader(model);

            GL.bindBuffer(GL.ARRAY_BUFFER, modelPiece.nrmBuffer);

            for (let desc of this.vertexAttributeDescs) {
                const loc = GL.getAttribLocation(this.program, desc.attrib);
                if (loc !== -1) {
                    GL.vertexAttribPointer(loc, desc.length, GL.FLOAT, false, desc.stride, desc.offset);
                    GL.enableVertexAttribArray(loc);
                }
            }

            GL.drawArrays(GL.LINES, 0, modelPiece.nrmVtxCount);
        }
    }

    get vertexAttributeDescs() {
        return ShaderP3C3.vertexAttributeDescs;
    }

    get vertexElementCount() {
        return 6;
    }
}

ShaderP3C3.vertexAttributeDescs = Object.freeze([ 
    {   attrib: 'a_pos',
        length: 3,
        stride: 24,
        offset: 0
    },
    {   attrib: 'a_col',
        length: 3,
        stride: 24,
        offset: 12
    }
]);

