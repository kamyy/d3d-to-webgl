class RefFrame {
    constructor(parent = null) {
        this.validSubtree = true;
        this.parent       = null;
        this.child        = null;
        this.next         = null;
        this.localM       = new Matrix4x4;
        this.modelM       = new Matrix4x4;

        if (parent instanceof RefFrame) {
            this.validSubtree  = false;
            this.parent        = parent;
            this.next          = parent.child;
            this.parent.child  = this;
        }
    }

    invalidateSubtree() {
        if (this.validSubtree) {
            for (let i = this.child; i !== null; i = i.next) {
                i.invalidateSubtree();
            }
            this.validSubtree = false;
            this.invalidateSubClass();
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

    get localMatrix() {
        return this.localM;
    }

    set localMatrix(matrix) {
        this.localM = matrix;
        this.invalidateSubtree();
    }

    get modelMatrix() {
        this.validateAscending();
        return this.modelM;
    }

    set modelMatrix(matrix) {
        // worldMatrix = localMatrix * parent->worldMatrix
        // worldMatrix / parent->worldMatrix = localMatrix
        this.localM = matrix.mul(this.parent.modelMatrix.inverse());
        invalidateSubtree();
    }

    translate(v, relative2) {
        if (relative2 === this || relative2 === null) { // relative to own axes
            const x = this.localM._41;
            const y = this.localM._42;
            const z = this.localM._43;
            this.localM._41 = x + (v.x * this.localM._11) + (v.y * this.localM._21) + (v.z * this.localM._31);
            this.localM._42 = y + (v.x * this.localM._12) + (v.y * this.localM._22) + (v.z * this.localM._32);
            this.localM._43 = z + (v.x * this.localM._13) + (v.y * this.localM._23) + (v.z * this.localM._33);

        } else if (relative2.parent === null) { // relative to root axes
            Vector1x4 d = relative2->mapPos(Vector1x4(v.x, v.y, v.z, 0.f), m_parent.get());
            this.localM._41 += d.m_x;
            this.localM._42 += d.m_y;
            this.localM._43 += d.m_z;

        } else if (relative2 === this.parent) { // relative to parent axes
            this.localM._41 += v.x;
            this.localM._42 += v.y;
            this.localM._43 += v.z;

        } else { // relative to arbitrary axes
            const d = relative2.mapPos(new Vector1x4(v.x, v.y, v.z, 0.0), this.parent);
            this.localM._41 += d.m_x;
            this.localM._42 += d.m_y;
            this.localM._43 += d.m_z;
        }
        this.invalidateSubtree();
    }

    rotateX(theta, relative2) {
        if (relative2 == this || relative2 == null) { // relative to own axes
            const rotx = new Matrix4x4; rotx.setRx(theta);
            this.localMatrix = rotx.mul(this.localMatrix);
        } else if (relative2 == m_parent.get()) { // relative to parent
            const rotx = new Matrix4x4; rotx.setRx(theta);
            this.localMatrix = this.localMatrix.mul(rotx);
        } else { // relative to arbitrary axes
            throw new Error('Cannot rotate relative to arbitrary axis');
        }
        invalidateSubtree();
    }

    rotateY(theta, relative2) {
        if (relative2 === this || relative2 === null) { // relative to own axes
            const roty = new Matrix4x4; roty.setRy(theta);
            this.localMatrix = roty.mul(this.localMatrix);
        } else if (relative2 == m_parent.get()) { // relative to parent
            const roty = new Matrix4x4; roty.setRy(theta);
            this.localMatrix = this.localMatrix.mul(roty);
        } else { // relative to arbitrary axes
            throw new Error('Cannot rotate relative to arbitrary axis');
        }
        invalidateSubtree();
    }

    rotateZ(theta, relative2) {
        if (relative2 === this || relative2 === null) { // relative to own axes
            const rotz = new Matrix4x4; rotz.setRz(theta);
            this.localMatrix = rotz.mul(this.localMatrix);
        } else if (relative2 == m_parent.get()) { // relative to parent
            const rotz = new Matrix4x4; rotz.setRz(theta);
            this.localMatrix = this.localMatrix.mul(rotz);
        } else { // relative to arbitrary axes
            throw new Error('Cannot rotate relative to arbitrary axis');
        }
        invalidateSubtree();
    }

    mapPos(v, tgt) {
        if (tgt == this.parent) {
            return v.mul(this.localMatrix);
        } else if (tgt.parent == null) {
            return v.mul(this.modelMatrix);
        } else {
            return v.mul(this.modelMatrix).mul(tgt.modelMatrix.inverse());
        }
    }
}

