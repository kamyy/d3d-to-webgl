import Shader from './Shader.js';

const vertexAttributeDescsP3N3 = Object.freeze([ 
    {   attrib: 'a_pos',
        length: 3,
        stride: 24,
        offset: 0
    },
    {   attrib: 'a_nrm',
        length: 3,
        stride: 24,
        offset: 12
    }
]);

export default class ShaderP3N3 extends Shader {
    constructor() {
        super('/glsl/P3N3.vert', '/glsl/P3N3.frag');
    }

    static get vertexAttributeDescs() {
        return vertexAttributeDescsP3N3;
    }

    static get vertexElementCount() {
        return 6;
    }
}
