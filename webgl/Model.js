class Model extends RefFrame {
    constructor(parent, node) {
        super(parent, node);

        if (node.hasOwnProperty('pieces')) {
            this.modelPieces = node.pieces;
            for (let piece of this.modelPieces) {
                piece.material  = g_GL.mapOfMaterials.get(piece.material);
                piece.nrmBuffer = g_GL.createBuffer();
                piece.vtxBuffer = g_GL.createBuffer();
                piece.idxBuffer = g_GL.createBuffer();

                g_GL.bindBuffer(g_GL.ARRAY_BUFFER, piece.nrmBuffer);
                g_GL.bufferData(g_GL.ARRAY_BUFFER, new Float32Array(piece.nrms), g_GL.STATIC_DRAW);

                g_GL.bindBuffer(g_GL.ARRAY_BUFFER, piece.vtxBuffer);
                g_GL.bufferData(g_GL.ARRAY_BUFFER, new Float32Array(piece.vtxs), g_GL.STATIC_DRAW);

                g_GL.bindBuffer(g_GL.ELEMENT_ARRAY_BUFFER, piece.idxBuffer);
                g_GL.bufferData(g_GL.ELEMENT_ARRAY_BUFFER, new Uint16Array(piece.idxs), g_GL.STATIC_DRAW);

                piece.nrmVtxCount = piece.nrms.length / ShaderP3C3.vertexAttributeDescs[0].stride;
                piece.triVtxCount = piece.idxs.length;
                piece.nrms = null;
                piece.vtxs = null;
                piece.idxs = null;
            };
        } else {
            this.modelPieces = [];
        }
    }

    drawModel() {
        for (let piece of this.modelPieces) {
            if (piece.material.isTranslucent) {
                piece.material.shader.drawTriangles(this, piece);
            } else {
                piece.material.shader.drawTriangles(this, piece);
            }
        }
    }
    
    drawMirror() {
        for (let piece of this.modelPieces) {
            piece.material.shader.drawTriangles(this, piece);
        }
    }

    drawNormals() {
        const shader = g_GL.mapOfShaders.get('P3C3');
        for (let piece of this.modelPieces) {
            shader.drawNormals(this, piece);
        }
    }
}

