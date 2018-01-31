class Shader {
    constructor(gl, filenameVS, filenameFS) {
        this.gl = gl;
        this.vs = null;
        this.fs = null;
        this.program = null;

        const createProgram = function(gl, vs, fs) {
            const program = gl.createProgram();
            gl.attachShader(program, vs);
            gl.attachShader(program, fs);
            gl.linkProgram(program);

            if (!gl.getProgramParameter(program, gl.LINK_STATUS)) {
                throw new Error('Error linking shader program!\n\n');
            }
            return program;
        }

        const requestVS = new XMLHttpRequest();
        requestVS.onreadystatechange = () => {
            if (requestVS.readyState === 4 && requestVS.status !== 404) {
                this.vs = gl.createShader(gl.VERTEX_SHADER);
                gl.shaderSource(this.vs, requestVS.responseText);
                gl.compileShader(this.vs);

                if (!gl.getShaderParameter(this.vs, gl.COMPILE_STATUS)) {
                    throw new Error('Cannot compile ' + filenameVS + ' !\n\n' + gl.getShaderInfoLog(this.vs));
                } 
                if (this.vs && this.fs && !this.program) {
                    this.program = createProgram(gl, this.vs, this.fs);
                }
            }
        }
        requestVS.open('GET', filenameVS, true);
        requestVS.send();

        const requestFS = new XMLHttpRequest();
        requestFS.onreadystatechange = () => {
            if (requestFS.readyState === 4 && requestFS.status !== 404) {
                this.fs = gl.createShader(gl.FRAGMENT_SHADER);
                gl.shaderSource(this.fs, requestFS.responseText);
                gl.compileShader(this.fs);

                if (!gl.getShaderParameter(this.fs, gl.COMPILE_STATUS)) {
                    throw new Error('Cannot compile ' + filenameFS + ' !\n\n' + gl.getShaderInfoLog(this.fs));
                } 
                if (this.vs && this.fs && !this.program) {
                    this.program = createProgram(gl, this.vs, this.fs);
                }
            }
        }
        requestFS.open('GET', filenameFS, true);
        requestFS.send();
    }

    drawPrimitives(subModel) {
        if (this.program) {
            this.gl.useProgram(this.program);
            
            // model view projection matrix
            // model space camera pos
            // model space omniLS pos

            const modelM = subModel.modelMatrix;              // model space to world space matrix
            const origin = new Vector1x4(0.0, 0.0, 0.0, 1.0); // camera space origin position
            const up     = new Vector1x4(0.0, 0.0, 1.0, 0.0); // world space up direction

            this.setModelSpaceUpDir(up.mul(modelM.inverse()));
            return true;
        }
        return false;
    }

    setModelViewProjMatrix(matrix) {
        const location = this.gl.getUniformLocation(this.program, 'u_model_view_proj_matrix');
        if (location && matrix instanceof Matrix4x4) {
            const array = new Float32Array([matrix._11, matrix._21, matrix._31, matrix_41,
                                            matrix._12, matrix._22, matrix._32, matrix_42,
                                            matrix._13, matrix._23, matrix._33, matrix_43,
                                            matrix._14, matrix._24, matrix._34, matrix_44]);
            this.gl.uniformMatrix4fv(location, false, array); // OpenGL is column major
            return true;
        }
        return false;
    }

    setModelSpaceCameraPos(pos) {
        const location = this.gl.getUniformLocation(this.program, 'u_camera_pos');
        if (location && pos instanceof Vector1x4) {
            this.gl.uniform3f(location, pos.x, pos.y, pos.z);
            return true;
        }
        return false;
    }

    setModelSpaceOmniLSPos(pos) {
        const location = this.gl.getUniformLocation(this.program, 'u_omniLS_pos');
        if (location && pos instanceof Vector1x4) {
            this.gl.uniform3f(location, pos.x, pos.y, pos.z);
            return true;
        }
        return false;
    }

    setModelSpaceUpDir(dir) {
        const location = this.gl.getUniformLocation(this.program, 'u_up_dir');
        if (location && dir instanceof Vector1x4) {
            this.gl.uniform3f(location, dir.x, dir.y, dir.z);
            return true;
        }
        return false;
    }
}

