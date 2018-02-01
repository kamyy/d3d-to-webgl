class Camera extends RefFrame {
    constructor(parent, fieldOfView, aspectRatio, clipDistanceN, clipDistanceF) {
        super(parent);
        this._fieldOfView = fieldOfView;
        this._aspectRatio = aspectRatio;
        this._clipDistanceN = clipDistanceN;
        this._clipDistanceF = clipDistanceF;
        this._viewMatrix = null;
        this._projMatrix = null;
    }

    get fieldOfView() {
        return this._fieldOfView;
    }

    get aspectRatio() {
        return this._aspectRatio;
    }

    get clipDistanceN() {
        return _clipDistanceN;
    }

    get clipDistanceF() {
        return _clipDistanceF;
    }

    set fieldOfView(fieldOfView) {
        this._fieldOfView = fieldOfView;
    }

    set aspectRatio(aspectRatio) {
        this._aspectRatio = aspectRatio;
    }

    set clipDistanceN(clipDistance) {
        this._clipDistanceN = clipDistance;
    }

    set clipDistanceF(float clipDistance) {
        this._clipDistanceF = clipDistance;
    }

    get viewMatrix() {
        // right-handed camera system where +ve y-axis points in view direction
        this._viewMatrix = this.modelMatrix.inverse();
        return this._viewMatrix;
    }

    get projMatrix() {
        /*
        // standard D3D left-handed projection matrix
        DirectX::XMMATRIX xm = DirectX::XMMatrixPerspectiveFovLH(m_fieldOfView,
                                                                 m_aspectRatio,
                                                                 m_clipDistanceN,
                                                                 m_clipDistanceF);
        memcpy(&m_projMatrix, &xm, sizeof(m_projMatrix));
        */

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

        return this._projMatrix; 
    }

    get viewProjMatrix() {
        return this.viewMatrix.mul(this.projMatrix);
    }
}

Camera.g_cameraArray = [];
Camera.g_camTgtArray = [];

