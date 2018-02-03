class ShaderP3N3 extends Shader {
    constructor() {
        super('P3N3', 'http://localhost:8888/glsl/P3N3.vs', 'http://localhost:8888/glsl/P3N3.fs');
    }

    get vertexComponentNames() {
        return ShaderP3N3.vertexComponentNames;
    }

    get vertexAttributeDescs() {
        return ShaderP3N3.vertexAttributeDescs;
    }
}

ShaderP3N3.vertexComponentNames = [
    'x', 'y', 'z', 'nx', 'ny', 'nz'
];

ShaderP3N3.vertexAttributeDescs = [ 
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
];
