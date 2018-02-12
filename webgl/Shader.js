const g_up        = new Vector1x4(0.0, 0.0, 1.0, 0.0); // up direction 
const g_origin    = new Vector1x4(0.0, 0.0, 0.0, 1.0); // origin

class Shader {
    constructor(vertShaderURL, fragShaderURL) {
        this.vs = null;
        this.fs = null;
        this.program = null;

        const initProgram = () => {
            const program = g_GL.createProgram();
            g_GL.attachShader(program, this.vs);
            g_GL.attachShader(program, this.fs);
            g_GL.linkProgram(program);

            if (!g_GL.getProgramParameter(program, g_GL.LINK_STATUS)) {
                throw new Error('Error linking shader program!\n');
            }
            return program;
        }

        const requestVS = new XMLHttpRequest();
        requestVS.onreadystatechange = () => {
            if (requestVS.readyState === 4 && requestVS.status === 200) {
                this.vs = g_GL.createShader(g_GL.VERTEX_SHADER);
                g_GL.shaderSource(this.vs, requestVS.responseText);
                g_GL.compileShader(this.vs);

                if (!g_GL.getShaderParameter(this.vs, g_GL.COMPILE_STATUS)) {
                    throw new Error('Error compling ' + vertShaderURL + ' !\n' + g_GL.getShaderInfoLog(this.vs));
                } 
                if (this.vs && this.fs && !this.program) {
                    this.program = initProgram();
                }
            }
        }
        requestVS.open('GET', vertShaderURL, true);
        requestVS.send();

        const requestFS = new XMLHttpRequest();
        requestFS.onreadystatechange = () => {
            if (requestFS.readyState === 4 && requestFS.status === 200) {
                this.fs = g_GL.createShader(g_GL.FRAGMENT_SHADER);
                g_GL.shaderSource(this.fs, requestFS.responseText);
                g_GL.compileShader(this.fs);

                if (!g_GL.getShaderParameter(this.fs, g_GL.COMPILE_STATUS)) {
                    throw new Error('Error compling ' + fragShaderURL + ' !\n' + g_GL.getShaderInfoLog(this.fs));
                } 
                if (this.vs && this.fs && !this.program) {
                    this.program = initProgram();
                }
            }
        }
        requestFS.open('GET', fragShaderURL, true);
        requestFS.send();
    }

    drawTriangles(model, modelPiece) {
        if (this.program) {
            const { material, vtxBuffer, idxBuffer } = modelPiece;

            g_GL.useProgram(this.program);
            this.setUniformVariablesInVertShader(model);
            this.setUniformVariablesInFragShader(model, material);
            g_GL.bindBuffer(g_GL.ARRAY_BUFFER,         vtxBuffer);
            g_GL.bindBuffer(g_GL.ELEMENT_ARRAY_BUFFER, idxBuffer);

            for (let desc of this.vertexAttributeDescs) {
                const loc = g_GL.getAttribLocation(this.program, desc.attrib);
                if (loc !== -1) {
                    g_GL.vertexAttribPointer(loc, desc.length, g_GL.FLOAT, false, desc.stride, desc.offset);
                    g_GL.enableVertexAttribArray(loc);
                }
            }

            g_GL.drawElements(g_GL_TRIANGLES, modelPiece.idxs.length, g_GL.UNSIGNED_SHORT, 0);
        }
    }

    setUniformVariablesInVertShader(model) {
        const loc0 = g_GL.getUniformLocation(this.program, 'u_attnCoeffs');
        if (loc0 && g_GL.omniDirLS instanceof OmniDirLS) {
            g_GL.uniform3f(loc0, g_GL.omniDirLS.coeff0, g_GL.omniDirLS.coeff1, g_GL.omniDirLS.coeff2);
        }

        const loc1 = g_GL.getUniformLocation(this.program, 'u_model_view_proj_matrix');
        if (loc1 && matrix instanceof Matrix4x4) {
            g_GL.uniformMatrix4fv(loc1, false, matrix.toFloat32Array()); // OpenGL stores array sequence in column-major format
        }

        const loc2 = g_GL.getUniformLocation(this.program, 'u_camera_pos');
        if (loc2 && g_GL.activeCamera instanceof Camera && model instanceof Model) {
            const pos = g_GL.activeCamera.map(g_origin, model);
            g_GL.uniform3f(loc2, pos.x, pos.y, pos.z);
        }

        const loc3 = g_GL.getUniformLocation(this.program, 'u_omniLS_pos');
        if (loc3 && g_GL.omniDirLS instanceof OmniDirLS && model instanceof Model) {
            const pos = g_GL.omniDirLS.map(g_origin, model);
            g_GL.uniform3f(loc3, pos.x, pos.y, pos.z);
        }
    }

    setUniformVariablesInFragShader(model, material) {
        const loc0 = g_GL.getUniformLocation(this.program, 'u_int');
        if (loc0 && g_GL.omniDirLS instanceof OmniDirLS) {
            g_GL.uniform3f(loc0, g_GL.omniDirLS.color[0], g_GL.omniDirLS.color[1], g_GL.omniDirLS.color[2]);
        }

        const loc1 = g_GL.getUniformLocation(this.program, 'u_gnd');
        if (loc1 && g_GL.ambientLS instanceof AmbientLS) {
            g_GL.uniform3f(loc1, g_GL.ambientLS.lowerHemisphereColor[0], g_GL.ambientLS.lowerHemisphereColor[1], g_GL.ambientLS.lowerHemisphereColor[2]);
        }

        const loc2 = g_GL.getUniformLocation(this.program, 'u_sky');
        if (loc2 && g_GL.ambientLS instanceof AmbientLS) {
            g_GL.uniform3f(loc2, g_GL.ambientLS.upperHemisphereColor[0], g_GL.ambientLS.upperHemisphereColor[1], g_GL.ambientLS.upperHemisphereColor[2]);
        }

        const loc3 = g_GL.getUniformLocation(this.program, 'u_ambi');
        if (loc3) {
            g_GL.uniform3f(loc3, material.diff[0], material.diff[1], material.diff[2]);
        }

        const loc4 = g_GL.getUniformLocation(this.program, 'u_diff');
        if (loc4 && g_GL.omniDirLS instanceof OmniDirLS) {
            const r = g_GL.omniDirLS.color[0] * material.diff[0];
            const g = g_GL.omniDirLS.color[1] * material.diff[1];
            const b = g_GL.omniDirLS.color[2] * material.diff[2];
            g_GL.uniform3f(loc4, r, g, b);
        }

        const loc5 = g_GL.getUniformLocation(this.program, 'u_spec');
        if (loc5 && g_GL.omniDirLS instanceof OmniDirLS) {
            const r = g_GL.omniDirLS.color[0] * material.spec[0];
            const g = g_GL.omniDirLS.color[1] * material.spec[1];
            const b = g_GL.omniDirLS.color[2] * material.spec[2];
            g_GL.uniform4f(loc5, r, g, b, material.shinyExponent);
        }

        const loc6 = g_GL.getUniformLocation(this.program, 'u_smpl');
        if (loc6 && material.textures[0]) {
            g_GL.activeTexture(g_GL.TEXTURE0);
            g_GL.bindTexture(g_GL.TEXTURE_2D, material.textures[0]);
            g_GL.uniform1i(loc6, 0);
        }

        const loc7 = g_GL.getUniformLocation(this.program, 'u_up_dir');
        if (loc7 && model instanceof Model) {
            const dir = g_up.mul(model.modelMatrix.inverse());
            g_GL.uniform3f(loc7, dir.x, dir.y, dir.z);
        }
    }
}

