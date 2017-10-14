////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
function degreesToRadians(degrees) {
    return (degrees * Math.PI / 180.0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
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
        return null;
    }

    sub(rhs) {
        if (rhs instanceof Vector1x4) {
            return new Vector1x4(this.x - rhs.x, this.y - rhs.y, this.z - rhs.z);
        }
        return null;
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
        return null;
    }

    div(rhs) {
        if (typeof rhs === "number") {
            return new Vector1x4(this.x / rhs, this.y / rhs, this.z / rhs);
        }
        return null;
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
        return null;
    }

    dot(rhs) {
        if (rhs instanceof Vector1x4) {
            return (this.x * rhs.x) + (this.y * rhs.y) + (this.z * rhs.z);
        }
        return null;
    }

    toString() {
        return "[Vector1x4 " + this.x + ", " + this.y + ", " + this.z + ", " + this.w + "]"; 
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
class Matrix4x4 {
    constructor(_11 = 1.0, _12 = 0.0, _13 = 0.0, _14 = 0.0,
                _21 = 0.0, _22 = 1.0, _23 = 0.0, _24 = 0.0,
                _31 = 0.0, _32 = 0.0, _33 = 1.0, _34 = 0.0,
                _41 = 0.0, _42 = 0.0, _43 = 0.0, _44 = 1.0) {
        this._11 = _11; this._12 = _12; this._13 = _13; this._14 = _14;
        this._21 = _21; this._22 = _22; this._23 = _23; this._24 = _24;
        this._31 = _31; this._32 = _32; this._33 = _33; this._34 = _34;
        this._41 = _41; this._42 = _42; this._43 = _43; this._44 = _44;
    }

    setRx(theta) {
        const cosTheta = Math.cos(theta);
        const sinTheta = Math.sin(theta);
        this._11 = 1.0;        this._12 = 0.0;        this._13 = 0.0;        this._14 = 0.0;
        this._21 = 0.0;        this._22 = cosTheta;   this._23 = sinTheta;   this._24 = 0.0;
        this._31 = 0.0;        this._32 = -sinTheta;  this._33 = cosTheta;   this._34 = 0.0;
        this._41 = 0.0;        this._42 = 0.0;        this._43 = 0.0;        this._44 = 1.0;
    }

    setRy(theta) {
        const cosTheta = Math.cos(theta);
        const sinTheta = Math.sin(theta);
        this._11 = cosTheta;   this._12 = 0.0;        this._13 = -sinTheta;  this._14 = 0.0;
        this._21 = 0.0;        this._22 = 1.0;        this._23 = 0.0;        this._24 = 0.0;
        this._31 = sinTheta;   this._32 = 0.0;        this._33 = cosTheta;   this._34 = 0.0;
        this._41 = 0.0;        this._42 = 0.0;        this._43 = 0.0;        this._44 = 1.0;
    }

    setRz(theta) {
        const cosTheta = Math.cos(theta);
        const sinTheta = Math.sin(theta);
        this._11 = cosTheta;   this._12 = sinTheta;   this._13 = 0.0;        this._14 = 0.0;
        this._21 = -sinTheta;  this._22 = cosTheta;   this._23 = 0.0;        this._24 = 0.0;
        this._31 = 0.0;        this._32 = 0.0;        this._33 = 1.0;        this._34 = 0.0;
        this._41 = 0.0;        this._42 = 0.0;        this._43 = 0.0;        this._44 = 1.0;
    }

    mul(rhs) {
        if (rhs instanceof Matrix4x4) {
            return new Matrix4x4(this._11 * rhs._11 + this._12 * rhs._21 + this._13 * rhs._31 + this._14 * rhs._41,
                                 this._11 * rhs._12 + this._12 * rhs._22 + this._13 * rhs._32 + this._14 * rhs._42,
                                 this._11 * rhs._13 + this._12 * rhs._23 + this._13 * rhs._33 + this._14 * rhs._43,
                                 this._11 * rhs._14 + this._12 * rhs._24 + this._13 * rhs._34 + this._14 * rhs._44,

                                 this._21 * rhs._11 + this._22 * rhs._21 + this._23 * rhs._31 + this._24 * rhs._41,
                                 this._21 * rhs._12 + this._22 * rhs._22 + this._23 * rhs._32 + this._24 * rhs._42,
                                 this._21 * rhs._13 + this._22 * rhs._23 + this._23 * rhs._33 + this._24 * rhs._43,
                                 this._21 * rhs._14 + this._22 * rhs._24 + this._23 * rhs._34 + this._24 * rhs._44,

                                 this._31 * rhs._11 + this._32 * rhs._21 + this._33 * rhs._31 + this._34 * rhs._41,
                                 this._31 * rhs._12 + this._32 * rhs._22 + this._33 * rhs._32 + this._34 * rhs._42,
                                 this._31 * rhs._13 + this._32 * rhs._23 + this._33 * rhs._33 + this._34 * rhs._43,
                                 this._31 * rhs._14 + this._32 * rhs._24 + this._33 * rhs._34 + this._34 * rhs._44,

                                 this._41 * rhs._11 + this._42 * rhs._21 + this._43 * rhs._31 + this._44 * rhs._41,
                                 this._41 * rhs._12 + this._42 * rhs._22 + this._43 * rhs._32 + this._44 * rhs._42,
                                 this._41 * rhs._13 + this._42 * rhs._23 + this._43 * rhs._33 + this._44 * rhs._43,
                                 this._41 * rhs._14 + this._42 * rhs._24 + this._43 * rhs._34 + this._44 * rhs._44);
        }
        return null;
    }

    inverse() {
        return new Matrix4x4(this._11, this._21, this._31, 0.0,
                             this._12, this._22, this._32, 0.0,
                             this._13, this._23, this._33, 0.0,
                             -(this._41 * this._11 + this._42 * this._12 + this._43 * this._13),
                             -(this._41 * this._21 + this._42 * this._22 + this._43 * this._23),
                             -(this._41 * this._31 + this._42 * this._32 + this._43 * this._33), 1.0);
    }

    postCatTxyz(tx, ty, tz) {
        if (typeof tx === "number" && 
            typeof ty === "number" && 
            typeof tz === "number") {
            return new Matrix4x4(this._11, this._12, this._13, this._14,
                                 this._21, this._22, this._23, this._24,
                                 this._31, this._32, this._33, this._34,
                                 this._41 + tx, this._42 + ty, this._43 + tz, this._44);
        }
        return null;
    }

    postCatSxyz(sx, sy, sz) {
        if (typeof sx === "number" && 
            typeof sy === "number" && 
            typeof sz === "number") {
            return new Matrix4x4(this._11 * sx, this._12 * sy, this._13 * sz, this._14,
                                 this._21 * sx, this._22 * sy, this._23 * sz, this._24,
                                 this._31 * sx, this._32 * sy, this._33 * sz, this._34,
                                 this._41 + sx, this._42 * sy, this._43 * sz, this._44);
        }
        return null;
    }

    toString() {
        return "[Matrix4x4 " + 
                this._11 + ", " + this._12 + ", " + this._13 + ", " + this._14 + ", " + 
                this._21 + ", " + this._22 + ", " + this._23 + ", " + this._24 + ", " + 
                this._31 + ", " + this._32 + ", " + this._33 + ", " + this._34 + ", " +
                this._41 + ", " + this._42 + ", " + this._43 + ", " + this._44 + "]";
    }
}

