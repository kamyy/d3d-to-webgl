class Color {
    constructor(r = 0.0, g = 0.0, b = 0.0, a = 1.0) {
        this.r = r; this.g = g; this.b = b; this.a = a;
    }

    mul(rhs) {
        if (rhs instanceof Color) {
            return new Color(this.r * rhs.r, this.g * rhs.g, this.b * rhs.b, this.a * rhs.a);
        }
        return new Color(this.r * rhs, this.g * rhs, this.b * rhs, this.a * rhs);
    }

    add(rhs) {
        if (rhs instanceof Color) {
            return new Color(this.r + rhs.r, this.g + rhs.g, this.b + rhs.b, this.a + rhs.a);
        }
        return new Color(this.r + rhs, this.g + rhs, this.b + rhs, this.a + rhs);
    }

    sub(rhs) {
        if (rhs instanceof Color) {
            return new Color(this.r - rhs.r, this.g - rhs.g, this.b - rhs.b, this.a - rhs.a);
        }
        return new Color(this.r - rhs, this.g - rhs, this.b - rhs, this.a - rhs);
    }
}
