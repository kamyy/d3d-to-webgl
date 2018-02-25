import Matrix4x4, {_11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44 } from './Matrix4x4.js';
import Vector1x4 from './Vector1x4.js';

export default class RefFrame {
    constructor(parent = null, node = null) {
        this.validSubtree = true;
        this.parent       = null;
        this.child        = null;
        this.next         = null;
        this.localM       = new Matrix4x4();
        this.modelM       = new Matrix4x4();

        if (parent instanceof RefFrame) {
            this.validSubtree  = false;
            this.parent        = parent;
            this.next          = parent.child;
            this.parent.child  = this;
        }

        if (node) {
           this.modelMatrix = new Matrix4x4(node.modelMatrix); 
        }
    }

    invalidateSubtree() {
        if (this.validSubtree) {
            for (let i = this.child; i !== null; i = i.next) {
                i.invalidateSubtree();
            }
            this.validSubtree = false;
        }
    }

    validateAscending() {
        if (!this.validSubtree) {
            if (this.parent) {
                this.parent.validateAscending();
                this.modelM = this.localM.mul(this.parent.modelM);
            }
            this.validSubtree = true;
        }
    }

    *children() {
        for (let sibling = this.child; sibling !== null; sibling = sibling.next) { yield sibling; }
    }

    get localMatrix() {
        return this.localM;
    }

    set localMatrix(matrix) {
        this.localM = new Matrix4x4(matrix);
        this.invalidateSubtree();
    }

    get modelMatrix() {
        this.validateAscending();
        return this.modelM;
    }

    set modelMatrix(matrix) {
        // worldMatrix = localMatrix * parent->worldMatrix
        // worldMatrix / parent->worldMatrix = localMatrix
        if (this.parent instanceof RefFrame) {
            this.localM = matrix.mul(this.parent.modelMatrix.inverse());
            this.invalidateSubtree();
        }
    }

    translate(v, relative2) {
        if (relative2 === this || !relative2) { // relative to own axes
            const x = this.localM.m[_41];
            const y = this.localM.m[_42];
            const z = this.localM.m[_43];
            this.localM.m[_41] = x + (v.x * this.localM.m[_11]) + (v.y * this.localM.m[_21]) + (v.z * this.localM.m[_31]);
            this.localM.m[_42] = y + (v.x * this.localM.m[_12]) + (v.y * this.localM.m[_22]) + (v.z * this.localM.m[_32]);
            this.localM.m[_43] = z + (v.x * this.localM.m[_13]) + (v.y * this.localM.m[_23]) + (v.z * this.localM.m[_33]);

        } else if (relative2.parent === null) { // relative to root axes
            const d = relative2.mapPos(new Vector1x4(v.x, v.y, v.z, 0.0), this.parent);
            this.localM.m[_41] += d.m_x;
            this.localM.m[_42] += d.m_y;
            this.localM.m[_43] += d.m_z;

        } else if (relative2 === this.parent) { // relative to parent axes
            this.localM.m[_41] += v.x;
            this.localM.m[_42] += v.y;
            this.localM.m[_43] += v.z;

        } else { // relative to arbitrary axes
            const d = relative2.mapPos(new Vector1x4(v.x, v.y, v.z, 0.0), this.parent);
            this.localM.m[_41] += d.m_x;
            this.localM.m[_42] += d.m_y;
            this.localM.m[_43] += d.m_z;
        }
        this.invalidateSubtree();
    }

    rotateX(theta, relative2) {
        if (relative2 === this || !relative2) { // relative to own axes
            const rotx = Matrix4x4.createRx(theta);
            this.localMatrix = rotx.mul(this.localMatrix);
        } else if (relative2 === this.parent) { // relative to parent
            const rotx = Matrix4x4.createRx(theta);
            this.localMatrix = this.localMatrix.mul(rotx);
        } else { // relative to arbitrary axes
            throw new Error('Cannot rotate relative to arbitrary axis!');
        }
        this.invalidateSubtree();
    }

    rotateY(theta, relative2) {
        if (relative2 === this || !relative2) { // relative to own axes
            const roty = Matrix4x4.createRy(theta);
            this.localMatrix = roty.mul(this.localMatrix);
        } else if (relative2 === this.parent) { // relative to parent
            const roty = Matrix4x4.createRy(theta);
            this.localMatrix = this.localMatrix.mul(roty);
        } else { // relative to arbitrary axes
            throw new Error('Cannot rotate relative to arbitrary axis!');
        }
        this.invalidateSubtree();
    }

    rotateZ(theta, relative2) {
        if (relative2 === this || !relative2) { // relative to own axes
            const rotz = Matrix4x4.createRz(theta);
            this.localMatrix = rotz.mul(this.localMatrix);
        } else if (relative2 === this.parent) { // relative to parent
            const rotz = Matrix4x4.createRz(theta);
            this.localMatrix = this.localMatrix.mul(rotz);
        } else { // relative to arbitrary axes
            throw new Error('Cannot rotate relative to arbitrary axis!');
        }
        this.invalidateSubtree();
    }

    mapPos(v, tgt) {
        if (tgt === this.parent) {
            return v.mul(this.localMatrix);
        } else if (tgt.parent === null) {
            return v.mul(this.modelMatrix);
        } else {
            return v.mul(this.modelMatrix).mul(tgt.modelMatrix.inverse());
        }
    }
}

