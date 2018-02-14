class ShaderP3N3 extends Shader {
    constructor() {
        super('http://localhost:8888/glsl/P3N3.vert', 'http://localhost:8888/glsl/P3N3.frag');
    }

    get vertexAttributeDescs() {
        return ShaderP3N3.vertexAttributeDescs;
    }
}

ShaderP3N3.vertexAttributeDescs = Object.freeze([ 
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
