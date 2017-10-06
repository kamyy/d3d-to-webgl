////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class Shader {
    constructor(gl, filenameVS, filenameFS) {
        this.gl = gl;
        this.vs = null;
        this.fs = null;
        this.program = null;

        const self = this;

        const initProgram = function() {
            self.program = gl.createProgram();
            gl.attachShader(self.program, self.vs);
            gl.attachShader(self.program, self.fs);
            gl.linkProgram(self.program);

            if (!gl.getProgramParameter(self.program, gl.LINK_STATUS)) {
                throw new Error('Error linking shader program!\n\n');
            }
        }

        const requestVS = new XMLHttpRequest();
        requestVS.onreadystatechange = function() {
            if (requestVS.readyState === 4 && requestVS.status !== 404) {
                this.vs = gl.createShader(gl.VERTEX_SHADER);
                gl.shaderSource(this.vs, requestVS.responseText);
                gl.compileShader(this.vs);

                if (!gl.getShaderParameter(this.vs, gl.COMPILE_STATUS)) {
                    throw new Error('Error compiling shader!\n\n' + gl.getShaderInfoLog(this.vs));
                } 
                if (this.fs && !this.program) {
                    initProgram(gl);
                }
            }
        }
        requestVS.open('GET', filenameVS, true);
        requestVS.send();

        const requestFS = new XMLHttpRequest();
        requestFS.onreadystatechange = function() {
            if (requestFS.readyState === 4 && requestFS.status !== 404) {
                this.fs = gl.createShader(gl.FRAGMENT_SHADER);
                gl.shaderSource(this.fs, requestFS.responseText);
                gl.compileShader(this.fs);

                if (!gl.getShaderParameter(this.fs, gl.COMPILE_STATUS)) {
                    throw new Error('Error compiling shader!\n\n' + gl.getShaderInfoLog(this.fs));
                } 
                if (this.vs && !this.program) {
                    initProgram(gl);
                }
            }
        }
        requestFS.open('GET', filenameFS, true);
        requestFS.send();
    }

    useProgram() {
        if (this.program) {
            gl.useProgram(this.program);
            return true;
        }
        return false;
    }

    setModelViewProjMatrix(matrix) {
        const location = this.gl.getUniformLocation(this.program, 'u_model_view_proj_matrix');
        if (location !== null && matrix instanceof Matrix4x4) {
            const array = new Float32Array([matrix._11, matrix._21, matrix._31, matrix_41,
                                            matrix._12, matrix._22, matrix._32, matrix_42,
                                            matrix._13, matrix._23, matrix._33, matrix_43,
                                            matrix._14, matrix._24, matrix._34, matrix_44]);
            this.gl.uniformMatrix4fv(location, false, array);
            return true;
        }
        return false;
    }

    setCameraPos(pos) {
        const location = this.gl.getUniformLocation(this.program, 'u_camera_pos');
        if (location !== null && pos instanceof Vector1x4) {
            this.gl.uniform3f(location, pos.x, pos.y, pos.z);
            return true;
        }
        return false;
    }

    setOmniLSPos(pos) {
        const location = this.gl.getUniformLocation(this.program, 'u_omniLS_pos');
        if (location !== null && pos instanceof Vector1x4) {
            this.gl.uniform3f(location, pos.x, pos.y, pos.z);
            return true;
        }
        return false;
    }

    setUpDir(dir) {
        const location = this.gl.getUniformLocation(this.program, 'u_up_dir');
        if (location !== null && dir instanceof Vector1x4) {
            this.gl.uniform3f(location, dir.x, dir.y, dir.z);
            return true;
        }
        return false;
    }
}

class Shader_P3C4 extends Shader { 
    constructor(gl) {
        super(gl, 'P3C4.vs.glsl', 'P3C4.fs.glsl');
    }

    use(vertexBuffer) {
        this.useProgram();
        const a_pos = this.gl.getAttribLocation(this.program, 'a_pos');
        const a_col = this.gl.getAttribLocation(this.program, 'a_col'); 

        if (a_pos !== null && a_col !== null) {
            this.gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
            this.gl.vertexAttribPointer(a_pos, 3, this.gl.FLOAT, false, 28, 0);
            this.gl.enableVertexAttribArray(a_pos);
            this.gl.vertexAttribPointer(a_col, 4, this.gl.FLOAT, false, 28, 12);
            this.gl.enableVertexAttribArray(a_col);
            return true;
        }
    }
}

/*
let vertices = new Float32Array([0.5, 0.5]);

const vertexBuffer = gl.createBuffer();
if (vertexBuffer) {
    gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
    gl.bufferData(gl.ARRAY_BUFFER, vertices, gl.STATIC_DRAW);
}
*/

