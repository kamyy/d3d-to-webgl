import RefFrame from './RefFrame';
import { GL } from './App';

export default class Model extends RefFrame {
    constructor(parent, node, scene) {
        super(parent, node);

        this.shaderP3C3 = GL.mapOfShaders.get('P3C3');
        this.isTheFloor = false;
        this.scene = scene;

        if (node.hasOwnProperty('pieces')) {
            this.modelPieces = node.pieces;
            for (let piece of this.modelPieces) {
                piece.material = scene.mapOfMaterials.get(piece.material);
                if (piece.material.name === 'floor') {
                    this.isTheFloor = true;
                }
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
        for (let piece of this.modelPieces) {
            if (this.scene.filteredMaterials.has(piece.material)) {
                this.shaderP3C3.drawNormals(this, piece);
            }
        }
    }

    drawPieces(forReflection, cacheTranslucentPiece) {
        for (let piece of this.modelPieces) {
            if (this.scene.filteredMaterials.has(piece.material)) {
                if (!forReflection && piece.material.isTranslucent) {
                    cacheTranslucentPiece({model:this, piece:piece});
                } else {
                    piece.material.shader.drawTriangles(this, piece);
                }
            }
        }
    }

    drawEdges() {
        for (let piece of this.modelPieces) {
            if (this.scene.filteredMaterials.has(piece.material)) {
                piece.material.shader.drawTriangleEdges(this, piece);
            }
        }
    }
}
