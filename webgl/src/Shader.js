import Vector1x4 from './Vector1x4';
import AmbientLS from './AmbientLS';
import OmniDirLS from './OmniDirLS';
import Camera from './Camera';
import Model from './Model';

import { g_GL } from './Render3D';
import { g_scene } from './App';

const g_up     = new Vector1x4(0.0, 0.0, 1.0, 0.0);
const g_origin = new Vector1x4(0.0, 0.0, 0.0, 1.0);

export default class Shader {
    constructor(vertShaderURL, fragShaderURL) {
        this.vs = null;
        this.fs = null;
        this.program = null;
        this.edgeBuffer = g_GL.createBuffer();

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

            g_GL.drawElements(g_GL.TRIANGLES, modelPiece.triVtxCount, g_GL.UNSIGNED_SHORT, 0);
        }
    }

    drawTriangleEdges(model, modelPiece) {
        const camPosition = g_scene.activeCamera.mapPos(g_origin, model); // map camera position into model space
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

        g_GL.useProgram(this.program);
        this.setUniformVariablesInVertShader(model);
        this.setUniformVariablesInFragShader(model, modelPiece.material);
        g_GL.bindBuffer(g_GL.ARRAY_BUFFER,         modelPiece.vtxBuffer);
        g_GL.bindBuffer(g_GL.ELEMENT_ARRAY_BUFFER, this.edgeBuffer);
        g_GL.bufferData(g_GL.ELEMENT_ARRAY_BUFFER, new Uint16Array(edgeIndices), g_GL.STATIC_DRAW);

        for (let desc of this.vertexAttributeDescs) {
            const loc = g_GL.getAttribLocation(this.program, desc.attrib);
            if (loc !== -1) {
                g_GL.vertexAttribPointer(loc, desc.length, g_GL.FLOAT, false, desc.stride, desc.offset);
                g_GL.enableVertexAttribArray(loc);
            }
        }

        g_GL.drawElements(g_GL.LINES, edgeIndices.length, g_GL.UNSIGNED_SHORT, 0);
    }

    setUniformVariablesInVertShader(model) {
        const loc0 = g_GL.getUniformLocation(this.program, 'u_attnCoeffs');
        if (loc0 && g_scene.omniDirLS instanceof OmniDirLS) {
            g_GL.uniform3f(loc0, g_scene.omniDirLS.coeff0, g_scene.omniDirLS.coeff1, g_scene.omniDirLS.coeff2);
        }

        const loc1 = g_GL.getUniformLocation(this.program, 'u_model_view_proj_matrix');
        if (loc1) {
            let modelViewProjMatrix = model.modelMatrix.mul(g_scene.activeCamera.viewProjMatrix);
            g_GL.uniformMatrix4fv(loc1, false, modelViewProjMatrix.toFloat32Array()); // OpenGL stores array sequence in column-major format
        }

        const loc2 = g_GL.getUniformLocation(this.program, 'u_camera_pos');
        if (loc2 && g_scene.activeCamera instanceof Camera && model instanceof Model) {
            const pos = g_scene.activeCamera.mapPos(g_origin, model);
            g_GL.uniform3f(loc2, pos.x, pos.y, pos.z);
        }

        const loc3 = g_GL.getUniformLocation(this.program, 'u_omniLS_pos');
        if (loc3 && g_scene.omniDirLS instanceof OmniDirLS && model instanceof Model) {
            const pos = g_scene.omniDirLS.mapPos(g_origin, model);
            g_GL.uniform3f(loc3, pos.x, pos.y, pos.z);
        }

        const loc4 = g_GL.getUniformLocation(this.program, 'u_up_dir');
        if (loc4 && model instanceof Model) {
            const worldUp = new Vector1x4(0, 0, 1, 0);
            const modelUp = worldUp.mul(model.modelMatrix.inverse());
            g_GL.uniform3f(loc4, modelUp.x, modelUp.y, modelUp.z);
        }
    }

    setUniformVariablesInFragShader(model, material) {
        const loc0 = g_GL.getUniformLocation(this.program, 'u_int');
        if (loc0 && g_scene.omniDirLS instanceof OmniDirLS) {
            g_GL.uniform3f(loc0, g_scene.omniDirLS.color[0], g_scene.omniDirLS.color[1], g_scene.omniDirLS.color[2]);
        }

        const loc1 = g_GL.getUniformLocation(this.program, 'u_gnd');
        if (loc1 && g_scene.ambientLS instanceof AmbientLS) {
            g_GL.uniform3f(loc1, g_scene.ambientLS.lowerHemisphereColor[0], g_scene.ambientLS.lowerHemisphereColor[1], g_scene.ambientLS.lowerHemisphereColor[2]);
        }

        const loc2 = g_GL.getUniformLocation(this.program, 'u_sky');
        if (loc2 && g_scene.ambientLS instanceof AmbientLS) {
            g_GL.uniform3f(loc2, g_scene.ambientLS.upperHemisphereColor[0], g_scene.ambientLS.upperHemisphereColor[1], g_scene.ambientLS.upperHemisphereColor[2]);
        }

        const loc3 = g_GL.getUniformLocation(this.program, 'u_ambi');
        if (loc3) {
            g_GL.uniform3f(loc3, material.diff[0], material.diff[1], material.diff[2]);
        }

        const loc4 = g_GL.getUniformLocation(this.program, 'u_diff');
        if (loc4 && g_scene.omniDirLS instanceof OmniDirLS) {
            const r = g_scene.omniDirLS.color[0] * material.diff[0];
            const g = g_scene.omniDirLS.color[1] * material.diff[1];
            const b = g_scene.omniDirLS.color[2] * material.diff[2];
            g_GL.uniform3f(loc4, r, g, b);
        }

        const loc5 = g_GL.getUniformLocation(this.program, 'u_spec');
        if (loc5 && g_scene.omniDirLS instanceof OmniDirLS) {
            const r = g_scene.omniDirLS.color[0] * material.spec[0];
            const g = g_scene.omniDirLS.color[1] * material.spec[1];
            const b = g_scene.omniDirLS.color[2] * material.spec[2];
            g_GL.uniform4f(loc5, r, g, b, material.shinyExponent);
        }

        const loc6 = g_GL.getUniformLocation(this.program, 'u_smpl');
        if (loc6 && material.textures[0]) {
            g_GL.activeTexture(g_GL.TEXTURE0);
            g_GL.bindTexture(g_GL.TEXTURE_2D, material.textures[0]);
            g_GL.uniform1i(loc6, 0);
        }

        const loc7 = g_GL.getUniformLocation(this.program, 'u_norm');
        if (loc7 && material.textures[1]) {
            g_GL.activeTexture(g_GL.TEXTURE1);
            g_GL.bindTexture(g_GL.TEXTURE_2D, material.textures[1]);
            g_GL.uniform1i(loc7, 1);
        }

        const loc8 = g_GL.getUniformLocation(this.program, 'u_up_dir');
        if (loc8 && model instanceof Model) {
            const dir = g_up.mul(model.modelMatrix.inverse());
            g_GL.uniform3f(loc8, dir.x, dir.y, dir.z);
        }
    }
}

