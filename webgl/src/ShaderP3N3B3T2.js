import Shader from './Shader';

export default class ShaderP3N3B3T2 extends Shader {
    constructor(app) {
        super('/glsl/P3N3B3T2.vert', '/glsl/P3N3B3T2.frag', app);
    }

    get vertexAttributeDescs() {
        return ShaderP3N3B3T2.vertexAttributeDescs;
    }

    get vertexElementCount() {
        return 11;
    }
}

ShaderP3N3B3T2.vertexAttributeDescs = Object.freeze([ 
    {   attrib: 'a_pos',
        length: 3,
        stride: 44,
        offset: 0
    },
    {   attrib: 'a_nrm',
        length: 3,
        stride: 44,
        offset: 12
    },
    {   attrib: 'a_bnm',
        length: 3,
        stride: 44,
        offset: 24
    },
    {   attrib: 'a_tex',
        length: 2,
        stride: 44,
        offset: 36
    }
]);
