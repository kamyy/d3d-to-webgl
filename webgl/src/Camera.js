import Matrix4x4 from './Matrix4x4.js';
import RefFrame from './RefFrame.js';

export default class Camera extends RefFrame {
    constructor(parent, node) {
        super(parent, node);
        this._fieldOfView = node.fieldOfView;
        this._aspectRatio = node.aspectRatio;
        this._clipDistanceN = node.clipDistanceN;
        this._clipDistanceF = node.clipDistanceF;
        this._fieldOfViewDegrees = node.fieldOfView / Math.PI * 180.0;
    }

    set fieldOfView(degrees) {
        this._fieldOfViewDegrees = degrees;
        this._fieldOfView = Math.PI / 180.0 * degrees;
    }

    set aspectRatio(val) {
        this._aspectRatio = val;
    }

    set clipDistanceN(val) {
        this._clipDistanceN = val;
    }

    set clipDistanceF(val) {
        this._clipDistanceF = val;
    }

    get fieldOfView() {
        return this._fieldOfViewDegrees;
    }

    get aspectRatio() {
        return this._aspectRatio;
    }

    get clipDistanceN() {
        return this._clipDistanceN;
    }

    get clipDistanceF() {
        return this._clipDistanceF;
    }

    get viewProjMatrix() {
        const y = 1.0 / Math.tan(this._fieldOfView * 0.5);
        const x = y / this._aspectRatio;
        const z = ( this._clipDistanceF                      ) / (this._clipDistanceF - this._clipDistanceN);
        const w = (-this._clipDistanceN * this._clipDistanceF) / (this._clipDistanceF - this._clipDistanceN);

        const projMatrix = new Matrix4x4([ x, 0, 0, 0,
                                           0, 0, z, 1,
                                           0, y, 0, 0,
                                           0, 0, w, 0 ]);

        return this.modelMatrix.inverse().mul(projMatrix);
    }
}
