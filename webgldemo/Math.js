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
            return new Vector1x4(this.x + rhs.x, this.y + rhs.y, this.z + rhs.z);
        }

        sub(rhs) {
            return new Vector1x4(this.x - rhs.x, this.y - rhs.y, this.z - rhs.z);
        }

        neg() {
            return new Vector1x4(-this.x, -this.y, -this.z);
        }

        mulMatrix4x4(rhs) {
            const x = this.x * rhs._11 + this.y * rhs._21 + this.z * rhs._31 + this.w * rhs._41;
            const y = this.x * rhs._12 + this.y * rhs._22 + this.z * rhs._32 + this.w * rhs._42;
            const z = this.x * rhs._13 + this.y * rhs._23 + this.z * rhs._33 + this.w * rhs._43;
            const w = this.x * rhs._14 + this.y * rhs._24 + this.z * rhs._34 + this.w * rhs._44;
            return new Vector1x4(x, y, z, w);
        }

        mulScalar(k) {
            return new Vector1x4(this.x * k, this.y * k, this.z * k);
        }

        divScalar(k) {
            return new Vector1x4(this.x / k, this.y / k, this.z / k);
        }

        normalize() {
            const l = 1.0 / this.magnitude();
            return new Vector1x4(this.x * l, this.y * l, this.z * l);
        }

        magnitude() {
            return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
        }

        cross(rhs) {
            return new Vector1x4(this.y * rhs.z - this.z * rhs.y,
                                 this.z * rhs.x - this.x * rhs.z,
                                 this.x * rhs.y - this.y * rhs.x);
        }

        dot(rhs) {
            return (this.x * rhs.x) + (this.y * rhs.y) + (this.z * rhs.z);
        }
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    class Matrix4x4 {
        constructor(_11 = 1.0, _12 = 0.0, _13 = 0.0, _14 = 0.0,
                           _21 = 1.0, _22 = 1.0, _23 = 0.0, _24 = 0.0,
                           _31 = 1.0, _32 = 0.0, _33 = 1.0, _34 = 0.0,
                           _41 = 1.0, _42 = 0.0, _43 = 0.0, _44 = 1.0) {
            this._11 = _11; this._12 = _12; this._13 = _13; this._14 = _14;
            this._21 = _21; this._22 = _22; this._23 = _23; this._24 = _24;
            this._31 = _31; this._32 = _32; this._33 = _33; this._34 = _34;
            this._41 = _41; this._42 = _42; this._43 = _43; this._44 = _44;
        }

        mulMatrix4x4(rhs) {
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

        inverse() {
            return new Matrix4x4(this._11, this._21, this._31, 0.0f,
                                 this._12, this._22, this._32, 0.0f,
                                 this._13, this._23, this._33, 0.0f,
                                 -(this._41 * this._11 + this._42 * this._12 + this._43 * this._13),
                                 -(this._41 * this._21 + this._42 * this._22 + this._43 * this._23),
                                 -(this._41 * this._31 + this._42 * this._32 + this._43 * this._33), 1.0f);
        }

        postCatTxyz(tx, ty, tz) {
            return new Matrix4x4(this._11, this._12, this._13, this._14,
                                 this._21, this._22, this._23, this._24,
                                 this._31, this._32, this._33, this._34,
                                 this._41 + tx, this._42 + ty, this._43 + tz, this._44);
        }

        postCatSxyz(sx, sy, sz) {
            return new Matrix4x4(this._11 * sx, this._12 * sy, this._13 * sz, this._14,
                                 this._21 * sx, this._22 * sy, this._23 * sz, this._24,
                                 this._31 * sx, this._32 * sy, this._33 * sz, this._34,
                                 this._41 + sx, this._42 * sy, this._43 * sz, this._44);
        }

    }

