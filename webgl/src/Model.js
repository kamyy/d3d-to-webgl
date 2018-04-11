// @flow

import Scene from './Scene';
import RefFrame from './RefFrame';
import ShaderP3C3 from './ShaderP3C3';
import { GL, reduxStore } from './App';

export default class Model extends RefFrame {
    shaderP3C3 : ShaderP3C3;
    isTheFloor : boolean;
    scene      : Scene;
    modelPieces: Array<Object>;

    constructor(parent: RefFrame, node: Object, scene: Scene) {
        super(parent, node);

        this.shaderP3C3 = GL.mapOfShaders.get('P3C3');
        this.isTheFloor = false;
        this.scene = scene;

        if (node.hasOwnProperty('pieces')) {
            this.modelPieces = node.pieces;
            for (let piece of this.modelPieces) {
                const nameOfMaterial: string = piece.material;

                if (nameOfMaterial === 'floor') {
                    this.isTheFloor = true;
                }
                piece.material = scene.mapOfMaterials.get(nameOfMaterial);
                piece.nrmBuffer = GL.createBuffer();
                piece.vtxBuffer = GL.createBuffer();
                piece.idxBuffer = GL.createBuffer();

                GL.bindBuffer(GL.ARRAY_BUFFER, piece.nrmBuffer);
                GL.bufferData(GL.ARRAY_BUFFER, new Float32Array(piece.nrms), GL.STATIC_DRAW);

                GL.bindBuffer(GL.ARRAY_BUFFER, piece.vtxBuffer);
                GL.bufferData(GL.ARRAY_BUFFER, new Float32Array(piece.vtxs), GL.STATIC_DRAW);

                GL.bindBuffer(GL.ELEMENT_ARRAY_BUFFER, piece.idxBuffer);
                GL.bufferData(GL.ELEMENT_ARRAY_BUFFER, new Uint16Array(piece.idxs), GL.STATIC_DRAW);

                piece.nrmVtxCount = piece.nrms.length / this.shaderP3C3.vertexElementCount;
                piece.triVtxCount = piece.idxs.length;
            }
        } else {
            this.modelPieces = [];
        }
    }

    drawNormals() {
        const {
            sceneArray,
            curSceneId,
        } = reduxStore.getState();

        const setOfMaterials = sceneArray[curSceneId].setOfMaterials;

        for (let piece of this.modelPieces) {
            if (setOfMaterials.has(piece.material)) {
                this.shaderP3C3.drawNormals(this, piece);
            }
        }
    }

    drawPieces(forReflection: number, cacheTranslucentPiece: (Object) => void) {
        const {
            sceneArray,
            curSceneId,
        } = reduxStore.getState();

        const setOfMaterials = sceneArray[curSceneId].setOfMaterials;

        for (let piece of this.modelPieces) {
            if (setOfMaterials.has(piece.material)) {
                if (!forReflection && piece.material.isTranslucent) {
                    cacheTranslucentPiece({model:this, piece:piece});
                } else {
                    piece.material.shader.drawTriangles(this, piece);
                }
            }
        }
    }

    drawEdges() {
        const {
            sceneArray,
            curSceneId,
        } = reduxStore.getState();

        const setOfMaterials = sceneArray[curSceneId].setOfMaterials;

        for (let piece of this.modelPieces) {
            if (setOfMaterials.has(piece.material)) {
                piece.material.shader.drawTriangleEdges(this, piece);
            }
        }
    }
}
