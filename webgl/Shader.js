const g_up        = new Vector1x4(0.0, 0.0, 1.0, 0.0); // up direction 
const g_origin    = new Vector1x4(0.0, 0.0, 0.0, 1.0); // origin
const g_shaderMap = new Map();

class Shader {
    constructor(name, vertShaderURL, fragShaderURL) {
        if (g_shaderMap.has(name)) {
            return g_shaderMap.get(name);
        }
        g_shaderMap.set(name, this);

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
            if (requestVS.readyState === 4 && requestVS.status !== 404) {
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
            if (requestFS.readyState === 4 && requestFS.status !== 404) {
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

    get vertexClass() {
        const keys = this.vertexComponentNames;

        return class {
            constructor(...vals) {
                if (keys.length !== vals.length) {
                    throw new Error('Number of vertex component names !== number of vertex component values.');
                }
                for (let i = 0; i < keys.length; ++i) {
                    this[keys[i]] = vals[i];
                }
            }
        };
    }

    drawPrimitives(subModel) {
        if (this.program) {
            const { model, material, vtxBuffer, idxBuffer } = subModel;

            g_GL.useProgram(this.program);

            this.setUniformAttenuationCoeffs(model);
            this.setUniformModelSpaceCameraPos(model);
            this.setUniformModelSpaceOmniLSPos(model);
            this.setUniformModelSpaceUpDir(model);

            this.setUniformLightingProperties();
            this.setUniformMaterialProperties(material);

            g_GL.bindBuffer(g_GL.ARRAY_BUFFER,         vtxBuffer);
            g_GL.bindBuffer(g_GL.ELEMENT_ARRAY_BUFFER, idxBuffer);

            for (desc of this.vertexAttributeDescs) {
                const location = g_GL.getAttribLocation(this.program, desc.attrib);
                if (location !== -1) {
                    g_GL.vertexAttribPointer(location, desc.length, g_GL.FLOAT, false, desc.stride, desc.offset);
                    g_GL.enableVertexAttribArray(location);
                }
            }

            g_GL.drawElements(g_GL_TRIANGLES, subModel.vtxBuffer.vtxCount, g_GL.UNSIGNED_SHORT, 0);
        }
    }

    setUniformAttenuationCoeffs() {
        const location = g_GL.getUniformLocation(this.program, 'u_attnCoeffs');
        if (location && g_webGLdemo.omniDirLS instanceof OmniDirLS) {
            g_GL.uniform3f(location, g_webGLdemo.omniDirLS.coeff0, g_webGLdemo.omniDirLS.coeff1, g_webGLdemo.omniDirLS.coeff2);
        }
    }

    setUniformModelViewProjMatrix(model) {
        const location = g_GL.getUniformLocation(this.program, 'u_model_view_proj_matrix');
        if (location && matrix instanceof Matrix4x4) {
            const array = new Float32Array([matrix._11, matrix._12, matrix._13, matrix_14,
                                            matrix._21, matrix._22, matrix._23, matrix_24,
                                            matrix._31, matrix._32, matrix._33, matrix_34,
                                            matrix._41, matrix._42, matrix._43, matrix_44]);
            g_GL.uniformMatrix4fv(location, false, array); // OpenGL stores array sequence in column-major format
        }
    }

    setUniformModelSpaceCameraPos(model) {
        const location = g_GL.getUniformLocation(this.program, 'u_camera_pos');
        if (location && g_webGLdemo.activeCamera instanceof Camera && model instanceof Model) {
            const pos = g_webGLdemo.activeCamera.map(g_origin, model);
            g_GL.uniform3f(location, pos.x, pos.y, pos.z);
        }
    }

    setUniformModelSpaceOmniLSPos(model) {
        const location = g_GL.getUniformLocation(this.program, 'u_omniLS_pos');
        if (location && g_webGLdemo.omniDirLS instanceof OmniDirLS && model instanceof Model) {
            const pos = g_webGLdemo.omniDirLS.map(g_origin, model);
            g_GL.uniform3f(location, pos.x, pos.y, pos.z);
        }
    }

    setUniformLightingProperties() {
        const loc0 = g_GL.getUniformLocation(this.program, 'u_int');
        if (loc0 && g_webGLdemo.omniDirLS instanceof OmniDirLS) {
            const c = g_webGLdemo.omniDirLS.color;
            g_GL.uniform3f(loc0, c.r, c.g, c.b);
        }

        const loc1 = g_GL.getUniformLocation(this.program, 'u_gnd');
        if (loc1 && g_webGLdemo.ambientLS instanceof AmbientLS) {
            const c = g_webGLdemo.ambientLS.lowerHemisphereColor;
            g_GL.uniform3f(loc1, c.r, c.g, c.b);
        }

        const loc2 = g_GL.getUniformLocation(this.program, 'u_sky');
        if (loc2 && g_webGLdemo.ambientLS instanceof AmbientLS) {
            const c = g_webGLdemo.ambientLS.upperHemisphereColor;
            g_GL.uniform3f(loc2, c.r, c.g, c.b);
        }
    }

    setUniformMaterialProperties(material) {
        const loc0 = g_GL.getUniformLocation(this.program, 'u_ambi');
        if (loc0) {
            g_GL.uniform3f(loc0, material.diff.r, material.diff.g, material.diff.b);
        }

        const loc1 = g_GL.getUniformLocation(this.program, 'u_diff');
        if (loc1 && g_webGLdemo.omniDirLS instanceof OmniDirLS) {
            const c = g_webGLdemo.omniDirLS.color.mul(material.diff);
            g_GL.uniform3f(loc1, c.r, c.g, c.b);
        }

        const loc2 = g_GL.getUniformLocation(this.program, 'u_spec');
        if (loc2 && g_webGLdemo.omniDirLS instanceof OmniDirLS) {
            const c = g_webGLdemo.omniDirLS.color.mul(material.spec);
            g_GL.uniform4f(loc2, c.r, c.g, c.b, material.shinyExponent);
        }

        const loc3 = g_GL.getUniformLocation(this.program, 'u_smpl');
        if (loc3 && material.textures[0]) {
            g_GL.activeTexture(g_GL.TEXTURE0);
            g_GL.bindTexture(g_GL.TEXTURE_2D, material.textures[0]);
            g_GL.uniform1i(loc3, 0);
        }
    }

    setUniformModelSpaceUpDir(model) {
        const location = g_GL.getUniformLocation(this.program, 'u_up_dir');
        if (location && model instanceof Model) {
            const dir = g_up.mul(model.modelMatrix.inverse());
            g_GL.uniform3f(location, dir.x, dir.y, dir.z);
        }
    }
}

