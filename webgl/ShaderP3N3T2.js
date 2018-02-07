class ShaderP3N3T2 extends Shader {
    constructor() {
        super('P3N3T2', 'http://localhost:8888/glsl/P3N3T2.vert', 'http://localhost:8888/glsl/P3N3T2.frag');
    }

    get vertexComponentNames() {
        return ShaderP3N3T2.vertexComponentNames;
    }

    get vertexAttributeDescs() {
        return ShaderP3N3T2.vertexAttributeDescs;
    }
}

ShaderP3N3T2.vertexComponentNames = [
    'x', 'y', 'z', 'nx', 'ny', 'nz', 's', 't'
];

ShaderP3N3T2.vertexAttributeDescs = [ 
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
];