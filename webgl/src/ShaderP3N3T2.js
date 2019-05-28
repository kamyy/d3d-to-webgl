// @flow

import Shader from './Shader.js';

const vertexAttributeDescsP3N3T2 = Object.freeze([ 
    {   attrib: 'a_pos',
        length: 3,
        stride: 32,
        offset: 0
    },
    {   attrib: 'a_nrm',
        length: 3,
        stride: 32,
        offset: 12
    },
    {   attrib: 'a_tex',
        length: 2,
        stride: 32,
        offset: 24
    }
]);

export default class ShaderP3N3T2 extends Shader {
    constructor() {
        super('/glsl/P3N3T2.vert', '/glsl/P3N3T2.frag');
    }

    static get vertexAttributeDescs(): Object[] {
        return vertexAttributeDescsP3N3T2;
    }

    static get vertexElementCount(): number {
        return 8;
    }
}
