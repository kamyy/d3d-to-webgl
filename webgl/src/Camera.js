import Matrix4x4 from './Matrix4x4';
import RefFrame from './RefFrame';

export default class Camera extends RefFrame {
    constructor(parent, node) {
        super(parent, node);
        this._fieldOfView = node.fieldOfView;
        this._aspectRatio = node.aspectRatio;
        this._clipDistanceN = node.clipDistanceN;
        this._clipDistanceF = node.clipDistanceF;
        this._fieldOfViewDegrees = node.fieldOfView / Math.PI * 180.0;
        this._dirty = true;
    }
    
    validateAscending() {
        if (!this.validSubtree) {
            this._dirty = true;
        }
        super.validateAscending();
    }

    set fieldOfView(degrees) {
        this._fieldOfViewDegrees = degrees;
        this._fieldOfView = Math.PI / 180.0 * degrees;
        this._dirty = true;
    }

    get fieldOfView() {
        return this._fieldOfViewDegrees;
    }

    set aspectRatio(val) {
        this._aspectRatio = val;
        this._dirty = true;
    }

    get aspectRatio() {
        return this._aspectRatio;
    }

    set clipDistanceN(val) {
        this._clipDistanceN = val;
        this._dirty = true;
    }

    get clipDistanceN() {
        return this._clipDistanceN;
    }

    set clipDistanceF(val) {
        this._clipDistanceF = val;
        this._dirty = true;
    }

    get clipDistanceF() {
        return this._clipDistanceF;
    }

    get viewProjMatrix() {
        if (this._dirty) {
            this._dirty = false;

            const y = 1.0 / Math.tan(this._fieldOfView * 0.5);
            const x = y / this._aspectRatio;
            const z = ( this._clipDistanceF                      ) / (this._clipDistanceF - this._clipDistanceN);
            const w = (-this._clipDistanceN * this._clipDistanceF) / (this._clipDistanceF - this._clipDistanceN);

            const projMatrix = new Matrix4x4([ x, 0, 0, 0,
                                               0, 0, z, 1,
                                               0, y, 0, 0,
                                               0, 0, w, 0 ]);

            this._viewProjMatrix = this.modelMatrix.inverse().mul(projMatrix);
        }
        return this._viewProjMatrix;
    }
}
