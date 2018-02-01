class Shader {
    static get(name) {
        return Shader.map.get(name);
    }

    constructor(name, filenameVS, filenameFS) {
        if (Shader.map.has(name)) {
            return Shader.map.get(name);
        }
        Shader.map.set(name, this);

        this.vs = null;
        this.fs = null;
        this.program = null;

        const createProgram = function(vs, fs) {
            const program = g_GL.createProgram();
            g_GL.attachShader(program, vs);
            g_GL.attachShader(program, fs);
            g_GL.linkProgram(program);

            if (!g_GL.getProgramParameter(program, g_GL.LINK_STATUS)) {
                throw new Error('Error linking shader program!\n\n');
            }
            return program;
        }

        const requestVS = new XMLHttpRequest();
        requestVS.onreadystatechange = () => {
            if (requestVS.readyState === 4 && requestVS.status !== 404) {
                this.vs = g_GL.createShader(g_GL.VERTEX_SHADER);
                g_GL.shaderSource(this.vs, requestVS.responseText);
                g_GL.compileShader(this.vs);

                if (!g_GL.getShaderParameter(this.vs, g_GL.COMPILE_STATUS)) {
                    throw new Error('Cannot compile ' + filenameVS + ' !\n\n' + g_GL.getShaderInfoLog(this.vs));
                } 
                if (this.vs && this.fs && !this.program) {
                    this.program = createProgram(g_GL, this.vs, this.fs);
                }
            }
        }
        requestVS.open('GET', filenameVS, true);
        requestVS.send();

        const requestFS = new XMLHttpRequest();
        requestFS.onreadystatechange = () => {
            if (requestFS.readyState === 4 && requestFS.status !== 404) {
                this.fs = g_GL.createShader(g_GL.FRAGMENT_SHADER);
                g_GL.shaderSource(this.fs, requestFS.responseText);
                g_GL.compileShader(this.fs);

                if (!g_GL.getShaderParameter(this.fs, g_GL.COMPILE_STATUS)) {
                    throw new Error('Cannot compile ' + filenameFS + ' !\n\n' + g_GL.getShaderInfoLog(this.fs));
                } 
                if (this.vs && this.fs && !this.program) {
                    this.program = createProgram(g_GL, this.vs, this.fs);
                }
            }
        }
        requestFS.open('GET', filenameFS, true);
        requestFS.send();
    }

    drawPrimitives(subModel) {
        if (this.program) {
            g_GL.useProgram(this.program);
            
            // model view projection matrix
            // model space camera pos
            // model space omniLS pos

            setModelSpaceUpDir(subModel);
            return true;
        }
        return false;
    }

    setModelViewProjMatrix(matrix) {
        const location = g_GL.getUniformLocation(this.program, 'u_model_view_proj_matrix');
        if (location && matrix instanceof Matrix4x4) {
            const array = new Float32Array([matrix._11, matrix._21, matrix._31, matrix_41,
                                            matrix._12, matrix._22, matrix._32, matrix_42,
                                            matrix._13, matrix._23, matrix._33, matrix_43,
                                            matrix._14, matrix._24, matrix._34, matrix_44]);
            g_GL.uniformMatrix4fv(location, false, array); // OpenGL is column major
            return true;
        }
        return false;
    }

    setModelSpaceCameraPos(pos) {
        const location = g_GL.getUniformLocation(this.program, 'u_camera_pos');
        if (location && pos instanceof Vector1x4) {
            g_GL.uniform3f(location, pos.x, pos.y, pos.z);
            return true;
        }
        return false;
    }

    setModelSpaceOmniLSPos(pos) {
        const location = g_GL.getUniformLocation(this.program, 'u_omniLS_pos');
        if (location && pos instanceof Vector1x4) {
            g_GL.uniform3f(location, pos.x, pos.y, pos.z);
            return true;
        }
        return false;
    }

    setModelSpaceUpDir(subModel) {
        const location = g_GL.getUniformLocation(this.program, 'u_up_dir');
        if (location && subModel instanceof RefFrame) {
            const modelUp = g_worldUp.mul(subModel.modelMatrix.inverse());
            g_GL.uniform3f(location, modelUp.x, modelUp.y, modelUp.z);
            return true;
        }
        return false;
    }
}

Shader.map = new Map();
