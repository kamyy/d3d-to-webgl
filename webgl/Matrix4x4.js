const _11 = 0; 
const _12 = 1; 
const _13 = 2; 
const _14 = 3;

const _21 = 4; 
const _22 = 5; 
const _23 = 6; 
const _24 = 7;

const _31 = 8; 
const _32 = 9; 
const _33 = 10; 
const _34 = 11;

const _41 = 12; 
const _42 = 13; 
const _43 = 14; 
const _44 = 15;

class Matrix4x4 {
    constructor(elements) {
        if (!elements) {
            this.m = Array.of(1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0);
        } else if (elements instanceof Matrix4x4) {
            this.m = Array.from(elements.m);
        } else if (Array.isArray(elements) && elements.length === 16) {
            this.m = Array.from(elements);
        } else {
            throw new Error('Cannot construct Matrix4x4!');
        }
    }

    static createId() {
        return new Matrix4x4;
    }

    static createRx(theta) {
        const cosTheta = Math.cos(theta);
        const sinTheta = Math.sin(theta);
        return new Matrix4x4([
            1.0,       0.0,        0.0, 0.0,
            0.0, +cosTheta,  +sinTheta, 0.0,
            0.0, -sinTheta,  +cosTheta, 0.0,
            0.0,       0.0,        0.0, 1.0
        ]);
    }

    static createRy(theta) {
        const cosTheta = Math.cos(theta);
        const sinTheta = Math.sin(theta);
        return new Matrix4x4([
            +cosTheta, 0.0, -sinTheta, 0.0,
            0.0,       1.0,       0.0, 0.0,
            +sinTheta, 0.0, +cosTheta, 0.0,
            0.0,       0.0,       0.0, 1.0
        ]);
    }

    static createRz(theta) {
        const cosTheta = Math.cos(theta);
        const sinTheta = Math.sin(theta);
        return new Matrix4x4([
            +cosTheta, +sinTheta, 0.0, 0.0,
            -sinTheta, +cosTheta, 0.0, 0.0,
                  0.0,       0.0, 1.0, 0.0,
                  0.0,       0.0, 0.0, 1.0
        ]);
    }

    mul(rhs) {
        if (rhs instanceof Matrix4x4) {
            return new Matrix4x4([
                this.m[_11] * rhs.m[_11] + this.m[_12] * rhs.m[_21] + this.m[_13] * rhs.m[_31] + this.m[_14] * rhs.m[_41],
                this.m[_11] * rhs.m[_12] + this.m[_12] * rhs.m[_22] + this.m[_13] * rhs.m[_32] + this.m[_14] * rhs.m[_42],
                this.m[_11] * rhs.m[_13] + this.m[_12] * rhs.m[_23] + this.m[_13] * rhs.m[_33] + this.m[_14] * rhs.m[_43],
                this.m[_11] * rhs.m[_14] + this.m[_12] * rhs.m[_24] + this.m[_13] * rhs.m[_34] + this.m[_14] * rhs.m[_44],

                this.m[_21] * rhs.m[_11] + this.m[_22] * rhs.m[_21] + this.m[_23] * rhs.m[_31] + this.m[_24] * rhs.m[_41],
                this.m[_21] * rhs.m[_12] + this.m[_22] * rhs.m[_22] + this.m[_23] * rhs.m[_32] + this.m[_24] * rhs.m[_42],
                this.m[_21] * rhs.m[_13] + this.m[_22] * rhs.m[_23] + this.m[_23] * rhs.m[_33] + this.m[_24] * rhs.m[_43],
                this.m[_21] * rhs.m[_14] + this.m[_22] * rhs.m[_24] + this.m[_23] * rhs.m[_34] + this.m[_24] * rhs.m[_44],

                this.m[_31] * rhs.m[_11] + this.m[_32] * rhs.m[_21] + this.m[_33] * rhs.m[_31] + this.m[_34] * rhs.m[_41],
                this.m[_31] * rhs.m[_12] + this.m[_32] * rhs.m[_22] + this.m[_33] * rhs.m[_32] + this.m[_34] * rhs.m[_42],
                this.m[_31] * rhs.m[_13] + this.m[_32] * rhs.m[_23] + this.m[_33] * rhs.m[_33] + this.m[_34] * rhs.m[_43],
                this.m[_31] * rhs.m[_14] + this.m[_32] * rhs.m[_24] + this.m[_33] * rhs.m[_34] + this.m[_34] * rhs.m[_44],

                this.m[_41] * rhs.m[_11] + this.m[_42] * rhs.m[_21] + this.m[_43] * rhs.m[_31] + this.m[_44] * rhs.m[_41],
                this.m[_41] * rhs.m[_12] + this.m[_42] * rhs.m[_22] + this.m[_43] * rhs.m[_32] + this.m[_44] * rhs.m[_42],
                this.m[_41] * rhs.m[_13] + this.m[_42] * rhs.m[_23] + this.m[_43] * rhs.m[_33] + this.m[_44] * rhs.m[_43],
                this.m[_41] * rhs.m[_14] + this.m[_42] * rhs.m[_24] + this.m[_43] * rhs.m[_34] + this.m[_44] * rhs.m[_44]
            ]);
        }
        throw new Error('RHS argument not a Matrix4x4!');
    }

    inverse() {
        return new Matrix4x4([
            this.m[_11], this.m[_21], this.m[_31], 0.0,
            this.m[_12], this.m[_22], this.m[_32], 0.0,
            this.m[_13], this.m[_23], this.m[_33], 0.0,
            -(this.m[_41] * this.m[_11] + this.m[_42] * this.m[_12] + this.m[_43] * this.m[_13]),
            -(this.m[_41] * this.m[_21] + this.m[_42] * this.m[_22] + this.m[_43] * this.m[_23]),
            -(this.m[_41] * this.m[_31] + this.m[_42] * this.m[_32] + this.m[_43] * this.m[_33]), 1.0
        ]);
    }

    postCatTxyz(tx, ty, tz) {
        if (typeof tx === "number" && 
            typeof ty === "number" && 
            typeof tz === "number") {
            return new Matrix4x4([
                this.m[_11], this.m[_12], this.m[_13], this.m[_14],
                this.m[_21], this.m[_22], this.m[_23], this.m[_24],
                this.m[_31], this.m[_32], this.m[_33], this.m[_34],
                this.m[_41] + tx, this.m[_42] + ty, this.m[_43] + tz, this.m[_44]
            ]);
        }
        throw new Error('RHS argument not a Matrix4x4!');
    }

    postCatSxyz(sx, sy, sz) {
        if (typeof sx === "number" && 
            typeof sy === "number" && 
            typeof sz === "number") {
            return new Matrix4x4([
                this.m[_11] * sx, this.m[_12] * sy, this.m[_13] * sz, this.m[_14],
                this.m[_21] * sx, this.m[_22] * sy, this.m[_23] * sz, this.m[_24],
                this.m[_31] * sx, this.m[_32] * sy, this.m[_33] * sz, this.m[_34],
                this.m[_41] + sx, this.m[_42] * sy, this.m[_43] * sz, this.m[_44]
            ]);
        }
        throw new Error('RHS argument not a Matrix4x4!');
    }

    toFloat32Array() {
        return new Float32Array(this.m);
    }

    toString() {
        return "[Matrix4x4 " + 
                this.m[_11] + ", " + this.m[_12] + ", " + this.m[_13] + ", " + this.m[_14] + ", " + 
                this.m[_21] + ", " + this.m[_22] + ", " + this.m[_23] + ", " + this.m[_24] + ", " + 
                this.m[_31] + ", " + this.m[_32] + ", " + this.m[_33] + ", " + this.m[_34] + ", " +
                this.m[_41] + ", " + this.m[_42] + ", " + this.m[_43] + ", " + this.m[_44] + "]";
    }
}

