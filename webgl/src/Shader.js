// @flow

import Model from './Model';
import Camera from './Camera';
import Vector1x4 from './Vector1x4';
import OmniDirLS from './OmniDirLS';

import { GL, reduxStore, sceneArray } from './App';

const g_up     = new Vector1x4(0.0, 0.0, 1.0, 0.0);
const g_origin = new Vector1x4(0.0, 0.0, 0.0, 1.0);

function createXHR(url: string): Object {
    return new Promise(function(resolve, reject) {
        const xhr = new XMLHttpRequest();

        xhr.onreadystatechange = function() {
            if (this.readyState === 4) {
                switch (this.status) {
                case 200:
                    resolve(this.responseText);
                    break;
                default:
                    reject({
                        status: this.status, 
                        url:    url
                    });
                    break;
                }
            }
        };

        xhr.open('GET', url, true);
        xhr.send();
    });
}

export default class Shader {
    program             : Object | null;
    edgeBuffer          : Object;
    vertexElementCount  : number;
    vertexAttributeDescs: Array<Object>;

    constructor(vertShaderURL: string, fragShaderURL: string) {
        this.program    = null;
        this.edgeBuffer = GL.createBuffer();

        const promise = Promise.all([
            createXHR(vertShaderURL),
            createXHR(fragShaderURL),
        ]);
 
        promise.then(responseTexts => {
            const vs = GL.createShader(GL.VERTEX_SHADER);
            const fs = GL.createShader(GL.FRAGMENT_SHADER);
            GL.shaderSource(vs, responseTexts[0]);
            GL.shaderSource(fs, responseTexts[1]);
            GL.compileShader(vs);
            GL.compileShader(fs);

            if (!GL.getShaderParameter(vs, GL.COMPILE_STATUS)) {
                throw new Error('Error compiling ' + vertShaderURL + ' !\n' + GL.getShaderInfoLog(vs));
            } 
            if (!GL.getShaderParameter(fs, GL.COMPILE_STATUS)) {
                throw new Error('Error compiling ' + fragShaderURL + ' !\n' + GL.getShaderInfoLog(fs));
            } 

            this.program = GL.createProgram();
            GL.attachShader(this.program, vs);
            GL.attachShader(this.program, fs);
            GL.linkProgram(this.program);

            if (!GL.getProgramParameter(this.program, GL.LINK_STATUS)) {
                throw new Error('Error linking shader program!\n');
            }
        });

        promise.catch(({status, url}) => {
            throw new Error(`Cannot GET ${url} status=${status}`);
        });
    }


    drawTriangles(model: Model, modelPiece: Object) {
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

    drawTriangleEdges(model: Model, modelPiece: Object) {
        const camPosition = sceneArray.curScene.activeCamera.mapPos(g_origin, model); // map camera position into model space
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

    setUniformVariablesInVertShader(model: Model) {
        const scene = sceneArray.curScene;
        const omniDirLS = scene.omniDirLS;
        const activeCam = scene.activeCamera;

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

    setUniformVariablesInFragShader(model: Model, material: Object) {
        const {
            sceneArray,
            curSceneId,
        } = reduxStore.getState();

        const {
            omniDirLS,
            lowerAmbientLS,
            upperAmbientLS,
        } = sceneArray[curSceneId];

        const loc0 = GL.getUniformLocation(this.program, 'u_int');
        if (loc0) {
            GL.uniform3f(loc0, omniDirLS[0], omniDirLS[1], omniDirLS[2]);
        }

        const loc1 = GL.getUniformLocation(this.program, 'u_gnd');
        if (loc1) {
            GL.uniform3f(loc1, lowerAmbientLS[0], lowerAmbientLS[1], lowerAmbientLS[2]);
        }

        const loc2 = GL.getUniformLocation(this.program, 'u_sky');
        if (loc2) {
            GL.uniform3f(loc2, upperAmbientLS[0], upperAmbientLS[1], upperAmbientLS[2]);
        }

        const loc3 = GL.getUniformLocation(this.program, 'u_ambi');
        if (loc3) {
            GL.uniform3f(loc3, material.diff[0], material.diff[1], material.diff[2]);
        }

        const loc4 = GL.getUniformLocation(this.program, 'u_diff');
        if (loc4) {
            const r = omniDirLS[0] * material.diff[0];
            const g = omniDirLS[1] * material.diff[1];
            const b = omniDirLS[2] * material.diff[2];
            GL.uniform3f(loc4, r, g, b);
        }

        const loc5 = GL.getUniformLocation(this.program, 'u_spec');
        if (loc5) {
            const r = omniDirLS[0] * material.spec[0];
            const g = omniDirLS[1] * material.spec[1];
            const b = omniDirLS[2] * material.spec[2];
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

