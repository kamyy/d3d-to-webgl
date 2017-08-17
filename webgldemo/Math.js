    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    function degressToRadians(degrees) {
        return (degrees * Math.PI / 180.0);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    function Vector1x4(x = 0.0, y = 0.0, z = 0.0, w = 1.0) {
        this.x = x;
        this.y = y;
        this.z = z;
        this.w = w;
    }

    Vector1x4.prototype = {
        constructor: Vector1x4,

        add: function(rhs) {
            return new Vector1x4(this.x + rhs.x, this.y + rhs.y, this.z + rhs.z);
        },

        sub: function(rhs) {
            return new Vector1x4(this.x - rhs.x, this.y - rhs.y, this.z - rhs.z);
        },

        neg: function() {
            return new Vector1x4(-this.x, -this.y, -this.z);
        },

        mulByMatrix: function(rhs) {
            const x = this.x * rhs._11 + this.y * rhs._21 + this.z * rhs._31 + this.w * rhs._41;
            const y = this.x * rhs._12 + this.y * rhs._22 + this.z * rhs._32 + this.w * rhs._42;
            const z = this.x * rhs._13 + this.y * rhs._23 + this.z * rhs._33 + this.w * rhs._43;
            const w = this.x * rhs._14 + this.y * rhs._24 + this.z * rhs._34 + this.w * rhs._44;
            return new Vector1x4(x, y, z, w);
        },

        mulByScalar: function(k) {
            return new Vector1x4(this.x * k, this.y * k, this.z * k);
        },

        divByScalar: function(k) {
            return new Vector1x4(this.x / k, this.y / k, this.z / k);
        },

        normalize: function() {
            const l = 1.0 / this.magnitude();
            return new Vector1x4(this.x * l, this.y * l, this.z * l);
        },

        magnitude: function() {
            return Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);
        },

        cross: function(rhs) {
            return new Vector1x4(this.y * rhs.z - this.z * rhs.y,
                                 this.z * rhs.x - this.x * rhs.z,
                                 this.x * rhs.y - this.y * rhs.x);
        },

        dot: function(rhs) {
            return (this.x * rhs.x) + (this.y * rhs.y) + (this.z * rhs.z);
        },
    };

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    function Matrix4x4(_11 = 1.0, _12 = 0.0, _13 = 0.0, _14 = 0.0,
                       _21 = 1.0, _22 = 1.0, _23 = 0.0, _24 = 0.0,
                       _31 = 1.0, _32 = 0.0, _33 = 1.0, _34 = 0.0,
                       _41 = 1.0, _42 = 0.0, _43 = 0.0, _44 = 1.0) {
        this._11 = _11; this._12 = _12; this._13 = _13; this._14 = _14;
        this._21 = _21; this._22 = _22; this._23 = _23; this._24 = _24;
        this._31 = _31; this._32 = _32; this._33 = _33; this._34 = _34;
        this._41 = _41; this._42 = _42; this._43 = _43; this._44 = _44;
    }

    Matrix4x4.prototype = {
        constructor: Matrix4x4

    };

