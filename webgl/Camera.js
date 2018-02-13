const CAMERA_DEFAULT_N_CLIP_DISTANCE = 0.50;
const CAMERA_DEFAULT_F_CLIP_DISTANCE = 100.;
const CAMERA_DEFAULT_FOV = 1.0;

class Camera extends RefFrame {
    constructor(parent, node) {
        super(parent, node);
        this.fieldOfView = node.fieldOfView;
        this.aspectRatio = node.aspectRatio;
        this.clipDistanceN = node.clipDistanceN;
        this.clipDistanceF = node.clipDistanceF;
        this.viewMatrix = null;
        this.projMatrix = null;
    }

    getViewMatrix() {
        // right-handed camera system where +ve y-axis points in view direction
        this.viewMatrix = this.modelMatrix.inverse();
        return this.viewMatrix;
    }

    getProjMatrix() {
        /*
        // standard D3D left-handed projection matrix
        DirectX::XMMATRIX xm = DirectX::XMMatrixPerspectiveFovLH(m_fieldOfView,
                                                                 m_aspectRatio,
                                                                 m_clipDistanceN,
                                                                 m_clipDistanceF);
        memcpy(&m_projMatrix, &xm, sizeof(m_projMatrix));

        // flip y and z vertex coordinates to convert from RHS to LHS
        // for D3D by switching around rows 2 and 3
        const x = this._projMatrix._21;
        const y = this._projMatrix._22;
        const z = this._projMatrix._23;
        const w = this._projMatrix._24;
        this._projMatrix.m_21 = this._projMatrix.m_31;
        this._projMatrix.m_22 = this._projMatrix.m_32;
        this._projMatrix.m_23 = this._projMatrix.m_33;
        this._projMatrix.m_24 = this._projMatrix.m_34;
        this._projMatrix.m_31 = x;
        this._projMatrix.m_32 = y;
        this._projMatrix.m_33 = z;
        this._projMatrix.m_34 = w;

        */
        return this.projMatrix; 
    }

    get viewProjMatrix() {
        return this.getViewMatrix.mul(this.getProjMatrix);
    }
}
