// @flow

import Model from './Model';
import Shader from './Shader';
import { GL } from './App';

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
    setUniformVariablesInVertShader: (Model) => void;
    setUniformVariablesInFragShader: (Model) => void;

    constructor() {
        super('/glsl/P3C3.vert', '/glsl/P3C3.frag');
    }

    drawNormals(model: Model, modelPiece: Object) {
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

    static get vertexAttributeDescs(): Object[] {
        return vertexAttributeDescsP3C3;
    }

    static get vertexElementCount(): number {
        return 6;
    }
}


