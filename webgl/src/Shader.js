import Model from './Model';
import Camera from './Camera';
import Vector1x4 from './Vector1x4';
import AmbientLS from './AmbientLS';
import OmniDirLS from './OmniDirLS';

import { GL } from './App';

const g_up     = new Vector1x4(0.0, 0.0, 1.0, 0.0);
const g_origin = new Vector1x4(0.0, 0.0, 0.0, 1.0);

export default class Shader {
    constructor(vertShaderURL, fragShaderURL, getCurrentScene) {
        this.vs = null;
        this.fs = null;
        this.program = null;
        this.getCurrentScene = getCurrentScene;
        this.edgeBuffer = GL.createBuffer();

        const initProgram = () => {
            const program = GL.createProgram();
            GL.attachShader(program, this.vs);
            GL.attachShader(program, this.fs);
            GL.linkProgram(program);

            if (!GL.getProgramParameter(program, GL.LINK_STATUS)) {
                throw new Error('Error linking shader program!\n');
            }
            return program;
        }

        const requestVS = new XMLHttpRequest();
        requestVS.onreadystatechange = () => {
            if (requestVS.readyState === 4 && requestVS.status === 200) {
                this.vs = GL.createShader(GL.VERTEX_SHADER);
                GL.shaderSource(this.vs, requestVS.responseText);
                GL.compileShader(this.vs);

                if (!GL.getShaderParameter(this.vs, GL.COMPILE_STATUS)) {
                    throw new Error('Error compling ' + vertShaderURL + ' !\n' + GL.getShaderInfoLog(this.vs));
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
                this.fs = GL.createShader(GL.FRAGMENT_SHADER);
                GL.shaderSource(this.fs, requestFS.responseText);
                GL.compileShader(this.fs);

                if (!GL.getShaderParameter(this.fs, GL.COMPILE_STATUS)) {
                    throw new Error('Error compling ' + fragShaderURL + ' !\n' + GL.getShaderInfoLog(this.fs));
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

            GL.useProgram(this.program);
            this.setUniformVariablesInVertShader(model);
            this.setUniformVariablesInFragShader(model, material);
            GL.bindBuffer(GL.ARRAY_BUFFER,         vtxBuffer);
            GL.bindBuffer(GL.ELEMENT_ARRAY_BUFFER, idxBuffer);

            for (let desc of this.vertexAttributeDescs) {
                const loc = GL.getAttribLocation(this.program, desc.attrib);
                if (loc !== -1) {
                    GL.vertexAttribPointer(loc, desc.length, GL.FLOAT, false, desc.stride, desc.offset);
                    GL.enableVertexAttribArray(loc);
                }
            }

            GL.drawElements(GL.TRIANGLES, modelPiece.triVtxCount, GL.UNSIGNED_SHORT, 0);
        }
    }

    drawTriangleEdges(model, modelPiece) {
        const camPosition = this.getCurrentScene().activeCamera.mapPos(g_origin, model); // map camera position into model space
        const edgeIndices = []; // each pair of indices represents a triangle edge

        const idxs = modelPiece.idxs; // array of indices, every three indices represents a triangle
        const vtxs = modelPiece.vtxs; // array of vertices, there are several contignuous numbers in a vertex 

        for (let i = 0; i < idxs.length; i += 3) {
            let j = idxs[i] * this.vertexElementCount; // index to 1st element of 1st vertex of triangle
            const vx = camPosition.x - vtxs[j++];
            const vy = camPosition.y - vtxs[j++];
            const vz = camPosition.z - vtxs[j++];
            const nx = vtxs[j++];
            const ny = vtxs[j++];
            const nz = vtxs[j];

            if (vx * nx + vy * ny + vz * nz > 0) { // back face cull using 1st vertex of triangle
                edgeIndices.push(idxs[i]);      // 1st edge of triangle
                edgeIndices.push(idxs[i + 1]);
                edgeIndices.push(idxs[i + 1]);  // 2nd edge of triangle
                edgeIndices.push(idxs[i + 2]);
                edgeIndices.push(idxs[i + 2]);  // 3rd edge of triangle
                edgeIndices.push(idxs[i]);
            }
        }

        GL.useProgram(this.program);
        this.setUniformVariablesInVertShader(model);
        this.setUniformVariablesInFragShader(model, modelPiece.material);
        GL.bindBuffer(GL.ARRAY_BUFFER,         modelPiece.vtxBuffer);
        GL.bindBuffer(GL.ELEMENT_ARRAY_BUFFER, this.edgeBuffer);
        GL.bufferData(GL.ELEMENT_ARRAY_BUFFER, new Uint16Array(edgeIndices), GL.STATIC_DRAW);

        for (let desc of this.vertexAttributeDescs) {
            const loc = GL.getAttribLocation(this.program, desc.attrib);
            if (loc !== -1) {
                GL.vertexAttribPointer(loc, desc.length, GL.FLOAT, false, desc.stride, desc.offset);
                GL.enableVertexAttribArray(loc);
            }
        }

        GL.drawElements(GL.LINES, edgeIndices.length, GL.UNSIGNED_SHORT, 0);
    }

    setUniformVariablesInVertShader(model) {
        const omniDirLS = this.getCurrentScene().omniDirLS;
        const activeCam = this.getCurrentScene().activeCamera;

        const loc0 = GL.getUniformLocation(this.program, 'u_attnCoeffs');
        if (loc0 && omniDirLS instanceof OmniDirLS) {
            GL.uniform3f(loc0, omniDirLS.coeff0, omniDirLS.coeff1, omniDirLS.coeff2);
        }

        const loc1 = GL.getUniformLocation(this.program, 'u_model_view_proj_matrix');
        if (loc1) {
            let modelViewProjMatrix = model.modelMatrix.mul(activeCam.viewProjMatrix);
            GL.uniformMatrix4fv(loc1, false, modelViewProjMatrix.toFloat32Array()); // OpenGL stores array sequence in column-major format
        }

        const loc2 = GL.getUniformLocation(this.program, 'u_camera_pos');
        if (loc2 && activeCam instanceof Camera && model instanceof Model) {
            const pos = activeCam.mapPos(g_origin, model);
            GL.uniform3f(loc2, pos.x, pos.y, pos.z);
        }

        const loc3 = GL.getUniformLocation(this.program, 'u_omniLS_pos');
        if (loc3 && omniDirLS instanceof OmniDirLS && model instanceof Model) {
            const pos = omniDirLS.mapPos(g_origin, model);
            GL.uniform3f(loc3, pos.x, pos.y, pos.z);
        }

        const loc4 = GL.getUniformLocation(this.program, 'u_up_dir');
        if (loc4 && model instanceof Model) {
            const worldUp = new Vector1x4(0, 0, 1, 0);
            const modelUp = worldUp.mul(model.modelMatrix.inverse());
            GL.uniform3f(loc4, modelUp.x, modelUp.y, modelUp.z);
        }
    }

    setUniformVariablesInFragShader(model, material) {
        const omniDirLS = this.getCurrentScene().omniDirLS;
        const ambientLS = this.getCurrentScene().ambientLS;

        const loc0 = GL.getUniformLocation(this.program, 'u_int');
        if (loc0 && omniDirLS instanceof OmniDirLS) {
            GL.uniform3f(loc0, omniDirLS.color[0], omniDirLS.color[1], omniDirLS.color[2]);
        }

        const loc1 = GL.getUniformLocation(this.program, 'u_gnd');
        if (loc1 && ambientLS instanceof AmbientLS) {
            GL.uniform3f(loc1, ambientLS.lowerHemisphereColor[0], ambientLS.lowerHemisphereColor[1], ambientLS.lowerHemisphereColor[2]);
        }

        const loc2 = GL.getUniformLocation(this.program, 'u_sky');
        if (loc2 && ambientLS instanceof AmbientLS) {
            GL.uniform3f(loc2, ambientLS.upperHemisphereColor[0], ambientLS.upperHemisphereColor[1], ambientLS.upperHemisphereColor[2]);
        }

        const loc3 = GL.getUniformLocation(this.program, 'u_ambi');
        if (loc3) {
            GL.uniform3f(loc3, material.diff[0], material.diff[1], material.diff[2]);
        }

        const loc4 = GL.getUniformLocation(this.program, 'u_diff');
        if (loc4 && omniDirLS instanceof OmniDirLS) {
            const r = omniDirLS.color[0] * material.diff[0];
            const g = omniDirLS.color[1] * material.diff[1];
            const b = omniDirLS.color[2] * material.diff[2];
            GL.uniform3f(loc4, r, g, b);
        }

        const loc5 = GL.getUniformLocation(this.program, 'u_spec');
        if (loc5 && omniDirLS instanceof OmniDirLS) {
            const r = omniDirLS.color[0] * material.spec[0];
            const g = omniDirLS.color[1] * material.spec[1];
            const b = omniDirLS.color[2] * material.spec[2];
            GL.uniform4f(loc5, r, g, b, material.shinyExponent);
        }

        const loc6 = GL.getUniformLocation(this.program, 'u_smpl');
        if (loc6 && material.textures[0]) {
            GL.activeTexture(GL.TEXTURE0);
            GL.bindTexture(GL.TEXTURE_2D, material.textures[0]);
            GL.uniform1i(loc6, 0);
        }

        const loc7 = GL.getUniformLocation(this.program, 'u_norm');
        if (loc7 && material.textures[1]) {
            GL.activeTexture(GL.TEXTURE1);
            GL.bindTexture(GL.TEXTURE_2D, material.textures[1]);
            GL.uniform1i(loc7, 1);
        }

        const loc8 = GL.getUniformLocation(this.program, 'u_up_dir');
        if (loc8 && model instanceof Model) {
            const dir = g_up.mul(model.modelMatrix.inverse());
            GL.uniform3f(loc8, dir.x, dir.y, dir.z);
        }
    }
}

