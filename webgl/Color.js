class Color {
    constructor(r = 0.0, g = 0.0, b = 0.0) {
        this.r = r; this.g = g; this.b = b;
    }

    mul(rhs) {
        if (rhs instanceof Color) {
            return new Color(this.r * rhs.r, this.g * rhs.g, this.b * rhs.b);
        }
        return new Color(this.r * rhs, this.g * rhs, this.b * rhs);
    }

    add(rhs) {
        if (rhs instanceof Color) {
            return new Color(this.r + rhs.r, this.g + rhs.g, this.b + rhs.b);
        }
        return new Color(this.r + rhs, this.g + rhs, this.b + rhs);
    }

    sub(rhs) {
        if (rhs instanceof Color) {
            return new Color(this.r - rhs.r, this.g - rhs.g, this.b - rhs.b);
        }
        return new Color(this.r - rhs, this.g - rhs, this.b - rhs);
    }
}
