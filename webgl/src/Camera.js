// @flow

import Matrix4x4 from './Matrix4x4';
import RefFrame from './RefFrame';

export default class Camera extends RefFrame {
    _fieldOfView: number;
    _aspectRatio: number;
    _clipDistanceN: number;
    _clipDistanceF: number;
    _fieldOfViewDegrees: number;

    constructor(parent: RefFrame, node: Object) {
        super(parent, node);
        this._fieldOfView = node.fieldOfView;
        this._aspectRatio = node.aspectRatio;
        this._clipDistanceN = node.clipDistanceN;
        this._clipDistanceF = node.clipDistanceF;
        this._fieldOfViewDegrees = node.fieldOfView / Math.PI * 180.0;
    }

    set fieldOfView(degrees: number) {
        this._fieldOfViewDegrees = degrees;
        this._fieldOfView = Math.PI / 180.0 * degrees;
    }

    set aspectRatio(val: number) {
        this._aspectRatio = val;
    }

    set clipDistanceN(val: number) {
        this._clipDistanceN = val;
    }

    set clipDistanceF(val: number) {
        this._clipDistanceF = val;
    }

    get fieldOfView(): number {
        return this._fieldOfViewDegrees;
    }

    get aspectRatio(): number {
        return this._aspectRatio;
    }

    get clipDistanceN(): number {
        return this._clipDistanceN;
    }

    get clipDistanceF(): number {
        return this._clipDistanceF;
    }

    get viewProjMatrix(): Matrix4x4 {
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
