import Shader from './Shader.js';

export default class ShaderP3N3T2 extends Shader {
    constructor() {
        super('http://localhost:8888/glsl/P3N3T2.vert', 'http://localhost:8888/glsl/P3N3T2.frag');
    }

    get vertexAttributeDescs() {
        return ShaderP3N3T2.vertexAttributeDescs;
    }

    get vertexElementCount() {
        return 8;
    }
}

ShaderP3N3T2.vertexAttributeDescs = Object.freeze([ 
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
