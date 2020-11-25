import Shader from './Shader.js';
import { GL } from './App.js';

const vertexAttributeDescsP3C3 = Object.freeze([ 
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

export default class ShaderP3C3 extends Shader { 
    constructor() {
        super('/glsl/P3C3.vert', '/glsl/P3C3.frag');
    }

    drawNormals(model, modelPiece) {
        if (this.program) {
            GL.useProgram(this.program);
            this.setUniformVariablesInVertShader(model);
            this.setUniformVariablesInFragShader(model);

            GL.bindBuffer(GL.ARRAY_BUFFER, modelPiece.nrmBuffer);

            for (let desc of this.constructor.vertexAttributeDescs) {
                const loc = GL.getAttribLocation(this.program, desc.attrib);
                if (loc !== -1) {
                    GL.vertexAttribPointer(loc, desc.length, GL.FLOAT, false, desc.stride, desc.offset);
                    GL.enableVertexAttribArray(loc);
                }
            }

            GL.drawArrays(GL.LINES, 0, modelPiece.nrmVtxCount);
        }
    }

    static get vertexAttributeDescs() {
        return vertexAttributeDescsP3C3;
    }

    static get vertexElementCount() {
        return 6;
    }
}


