class Model extends RefFrame {
    constructor(parent, jsonModel) {
        super(parent);
        
        this.modelMatrix = new Matrix4x4(jsonModel.modelMatrix);
        this.modelPieces = jsonModel.pieces;

        for (let piece of this.pieces) {
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

            piece.nrmVtxCount = nrms.length;
            piece.triVtxCount = idxs.length;
            piece.nrms = null;
            piece.vtxs = null;
            piece.idxs = null;
        };
    }

    draw(noAlpha) {
        for (let piece of this.modelPieces) {
            if (noAlpha || !piece.material.isTranslucent) {
                piece.material.shader.drawTriangles(this, piece);
            } else {

            }
        }
    }

    drawNormals() {
        const shader = g_GL.mapOfShaders.get('P3C4');
        for (let piece of this.modelPieces) {
            shader.drawNormals(this, piece);
        }
    }
}

