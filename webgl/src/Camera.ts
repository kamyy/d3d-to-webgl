import Matrix4x4 from "./Matrix4x4"
import RefFrame from "./RefFrame"
import { SceneNodeJson } from "./types/scene"

export default class Camera extends RefFrame {
  private _fieldOfView: number
  private _aspectRatio: number
  private _clipDistanceN: number
  private _clipDistanceF: number
  private _fieldOfViewDegrees: number

  constructor(parent: RefFrame, node: SceneNodeJson) {
    super(parent, node)
    this._fieldOfView = node.fieldOfView ?? Math.PI / 4
    this._aspectRatio = node.aspectRatio ?? 16 / 9
    this._clipDistanceN = node.clipDistanceN ?? 0.1
    this._clipDistanceF = node.clipDistanceF ?? 1000
    this._fieldOfViewDegrees = (this._fieldOfView / Math.PI) * 180.0
  }

  set fieldOfView(degrees: number) {
    this._fieldOfViewDegrees = degrees
    this._fieldOfView = (Math.PI / 180.0) * degrees
  }

  set aspectRatio(val: number) {
    this._aspectRatio = val
  }

  set clipDistanceN(val: number) {
    this._clipDistanceN = val
  }

  set clipDistanceF(val: number) {
    this._clipDistanceF = val
  }

  get fieldOfView() {
    return this._fieldOfViewDegrees
  }

  get aspectRatio() {
    return this._aspectRatio
  }

  get clipDistanceN() {
    return this._clipDistanceN
  }

  get clipDistanceF() {
    return this._clipDistanceF
  }

  get viewProjMatrix() {
    const y = 1.0 / Math.tan(this._fieldOfView * 0.5)
    const x = y / this._aspectRatio
    const z = this._clipDistanceF / (this._clipDistanceF - this._clipDistanceN)
    const w = (-this._clipDistanceN * this._clipDistanceF) / (this._clipDistanceF - this._clipDistanceN)

    const projMatrix = new Matrix4x4([x, 0, 0, 0, 0, 0, z, 1, 0, y, 0, 0, 0, 0, w, 0])

    return this.modelMatrix.inverse().mul(projMatrix)
  }
}
