class Vector1x4 {
    constructor(x = 0.0, y = 0.0, z = 0.0, w = 1.0) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

    add(rhs) {
        if (rhs instanceof Vector1x4) {
            return new Vector1x4(this.x + rhs.x, this.y + rhs.y, this.z + rhs.z);
        }
        throw new Error('rhs argument not a Vector1x4!');
    }

    sub(rhs) {
        if (rhs instanceof Vector1x4) {
            return new Vector1x4(this.x - rhs.x, this.y - rhs.y, this.z - rhs.z);
        }
        throw new Error('rhs argument not a Vector1x4!');
    }

    neg() {
        return new Vector1x4(-this.x, -this.y, -this.z);
    }

    mul(rhs) {
        if (rhs instanceof Matrix4x4) {
            const x = this.x * rhs._11 + this.y * rhs._21 + this.z * rhs._31 + this.w * rhs._41;
            const y = this.x * rhs._12 + this.y * rhs._22 + this.z * rhs._32 + this.w * rhs._42;
            const z = this.x * rhs._13 + this.y * rhs._23 + this.z * rhs._33 + this.w * rhs._43;
            const w = this.x * rhs._14 + this.y * rhs._24 + this.z * rhs._34 + this.w * rhs._44;
            return new Vector1x4(x, y, z, w);
        }
        if (typeof rhs === "number") {
            return new Vector1x4(this.x * rhs, this.y * rhs, this.z * rhs);
        }
        throw new Error('rhs argument not a Vector1x4!');
    }

    div(rhs) {
        if (typeof rhs === "number") {
            return new Vector1x4(this.x / rhs, this.y / rhs, this.z / rhs);
        }
        throw new Error('rhs argument not a Vector1x4!');
    }

    normalize() {
        const l = 1.0 / this.magnitude();
        return new Vector1x4(this.x * l, this.y * l, this.z * l);
    }

    magnitude() {
        return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
    }

    cross(rhs) {
        if (rhs instanceof Vector1x4) {
            return new Vector1x4(this.y * rhs.z - this.z * rhs.y,
                                 this.z * rhs.x - this.x * rhs.z,
                                 this.x * rhs.y - this.y * rhs.x);
        }
        throw new Error('rhs argument not a Vector1x4!');
    }

    dot(rhs) {
        if (rhs instanceof Vector1x4) {
            return (this.x * rhs.x) + (this.y * rhs.y) + (this.z * rhs.z);
        }
        throw new Error('rhs argument not a Vector1x4!');
    }

    toString() {
        return "[Vector1x4 " + this.x + ", " + this.y + ", " + this.z + ", " + this.w + "]"; 
    }
}

