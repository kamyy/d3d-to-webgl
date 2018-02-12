class Model extends RefFrame {
    constructor(parent, jsonModel) {
        super(parent);
        
        this.modelMatrix = new Matrix4x4(jsonModel.modelMatrix);
        this.modelPieces = jsonModel.pieces;

        this.modelPieces.forEach(modelPiece => {
            modelPiece.material = g_GL.mapOfMaterials.get(modelPiece.material);
            modelPiece.nrmBuffer = g_GL.createBuffer();
            modelPiece.vtxBuffer = g_GL.createBuffer();
            modelPiece.idxBuffer = g_GL.createBuffer();

            g_GL.bindBuffer(g_GL.ARRAY_BUFFER, modelPiece.nrmBuffer);
            g_GL.bufferData(g_GL.ARRAY_BUFFER, new Float32Array(modelPiece.nrms), g_GL.STATIC_DRAW);

            g_GL.bindBuffer(g_GL.ARRAY_BUFFER, modelPiece.vtxBuffer);
            g_GL.bufferData(g_GL.ARRAY_BUFFER, new Float32Array(modelPiece.vtxs), g_GL.STATIC_DRAW);

            g_GL.bindBuffer(g_GL.ELEMENT_ARRAY_BUFFER, modelPiece.idxBuffer);
            g_GL.bufferData(g_GL.ELEMENT_ARRAY_BUFFER, new Uint16Array(idxs), g_GL.STATIC_DRAW);
        });
    }

    draw(noAlpha) {
        this.modelPieces.forEach(modelPiece => {
            if (noAlpha || !modelPiece.material.isTranslucent) {
                modelPiece.material.shader.drawTriangles(this, modelPiece);
            } else {

            }
        });
    }

    drawNormals() {
        const shaderP3C4 = g_GL.mapOfShaders.get('P3C4');
        this.modelPieces.forEach(modelPiece => {
            shaderP3C4.drawNormals(modelPiece);
        });
    }
}

