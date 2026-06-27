import Matrix4x4, { _11, _12, _13, _21, _22, _23, _31, _32, _33, _41, _42, _43 } from "../math/Matrix4x4"
import Vector1x4 from "../math/Vector1x4"
import { SceneNodeJson } from "../types/scene"

export default class RefFrame {
  validSubtree: boolean
  parent: RefFrame | null
  child: RefFrame | null
  next: RefFrame | null
  localM: Matrix4x4
  modelM: Matrix4x4

  constructor(parent: RefFrame | null = null, node: SceneNodeJson | null = null) {
    this.validSubtree = true
    this.parent = null
    this.child = null
    this.next = null
    this.localM = new Matrix4x4()
    this.modelM = new Matrix4x4()

    if (parent instanceof RefFrame) {
      this.validSubtree = false
      this.parent = parent
      this.next = parent.child
      this.parent.child = this
    }

    if (node?.modelMatrix) {
      this.modelMatrix = new Matrix4x4(node.modelMatrix)
    }
  }

  invalidateSubtree() {
    if (this.validSubtree) {
      for (let i = this.child; i !== null; i = i.next) {
        i.invalidateSubtree()
      }
      this.validSubtree = false
    }
  }

  validateAscending() {
    if (!this.validSubtree) {
      if (this.parent) {
        this.parent.validateAscending()
        this.modelM = this.localM.mul(this.parent.modelM)
      }
      this.validSubtree = true
    }
  }

  *children(): Generator<RefFrame> {
    for (let sibling = this.child; sibling !== null; sibling = sibling.next) {
      yield sibling
    }
  }

  get localMatrix() {
    return this.localM
  }

  set localMatrix(matrix: Matrix4x4) {
    this.localM = new Matrix4x4(matrix)
    this.invalidateSubtree()
  }

  get modelMatrix() {
    this.validateAscending()
    return this.modelM
  }

  set modelMatrix(matrix: Matrix4x4) {
    if (this.parent instanceof RefFrame) {
      this.localM = matrix.mul(this.parent.modelMatrix.inverse())
      this.invalidateSubtree()
    }
  }

  translate(v: Vector1x4, relative2?: RefFrame) {
    if (relative2 === this || !relative2) {
      const x = this.localM.m[_41]
      const y = this.localM.m[_42]
      const z = this.localM.m[_43]
      this.localM.m[_41] = x + v.x * this.localM.m[_11] + v.y * this.localM.m[_21] + v.z * this.localM.m[_31]
      this.localM.m[_42] = y + v.x * this.localM.m[_12] + v.y * this.localM.m[_22] + v.z * this.localM.m[_32]
      this.localM.m[_43] = z + v.x * this.localM.m[_13] + v.y * this.localM.m[_23] + v.z * this.localM.m[_33]
    } else if (relative2.parent === null) {
      const d = relative2.mapPos(new Vector1x4(v.x, v.y, v.z, 0.0), this.parent ?? undefined)
      this.localM.m[_41] += d.x
      this.localM.m[_42] += d.y
      this.localM.m[_43] += d.z
    } else if (relative2 === this.parent) {
      this.localM.m[_41] += v.x
      this.localM.m[_42] += v.y
      this.localM.m[_43] += v.z
    } else {
      const d = relative2.mapPos(new Vector1x4(v.x, v.y, v.z, 0.0), this.parent ?? undefined)
      this.localM.m[_41] += d.x
      this.localM.m[_42] += d.y
      this.localM.m[_43] += d.z
    }
    this.invalidateSubtree()
  }

  rotateX(theta: number, relative2?: RefFrame) {
    if (relative2 === this || !relative2) {
      const rotx = Matrix4x4.createRx(theta)
      this.localMatrix = rotx.mul(this.localMatrix)
    } else if (relative2 === this.parent) {
      const rotx = Matrix4x4.createRx(theta)
      this.localMatrix = this.localMatrix.mul(rotx)
    } else {
      throw new Error("Cannot rotate relative to arbitrary axis!")
    }
    this.invalidateSubtree()
  }

  rotateY(theta: number, relative2?: RefFrame) {
    if (relative2 === this || !relative2) {
      const roty = Matrix4x4.createRy(theta)
      this.localMatrix = roty.mul(this.localMatrix)
    } else if (relative2 === this.parent) {
      const roty = Matrix4x4.createRy(theta)
      this.localMatrix = this.localMatrix.mul(roty)
    } else {
      throw new Error("Cannot rotate relative to arbitrary axis!")
    }
    this.invalidateSubtree()
  }

  rotateZ(theta: number, relative2?: RefFrame) {
    if (relative2 === this || !relative2) {
      const rotz = Matrix4x4.createRz(theta)
      this.localMatrix = rotz.mul(this.localMatrix)
    } else if (relative2 === this.parent) {
      const rotz = Matrix4x4.createRz(theta)
      this.localMatrix = this.localMatrix.mul(rotz)
    } else {
      throw new Error("Cannot rotate relative to arbitrary axis!")
    }
    this.invalidateSubtree()
  }

  mapPos(v: Vector1x4, tgt?: RefFrame) {
    if (tgt === this.parent) {
      return v.mul(this.localMatrix)
    }
    if (tgt && tgt.parent === null) {
      return v.mul(this.modelMatrix)
    }
    if (tgt) {
      return v.mul(this.modelMatrix).mul(tgt.modelMatrix.inverse())
    }
    return v.mul(this.modelMatrix)
  }
}
