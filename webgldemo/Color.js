class Color {
    constructor(float r = 0.0, float g = 0.0, float b = 0.0, float a = 1.0) {
        this.r = r;
        this.g = g;
        this.b = b;
        this.a = a;
    }

    mul(rhs) {
        if (rhs instanceof Color) {
            return new Color(this.r * rhs.r, 
                             this.g * rhs.g, 
                             this.b * rhs.b, 
                             this.a * rhs.a);
        }
        return new Color(this.r * rhs, 
                         this.g * rhs, 
                         this.b * rhs, 
                         this.a * rhs);
    }

    add(rhs) {
        if (rhs instanceof Color) {
            return new Color(this.r + rhs.r, 
                             this.g + rhs.g, 
                             this.b + rhs.b, 
                             this.a + rhs.a);
        }
        return new Color(this.r + rhs, 
                         this.g + rhs, 
                         this.b + rhs, 
                         this.a + rhs);
    }

    sub(rhs) {
        if (rhs instanceof Color) {
            return new Color(this.r - rhs.r, 
                             this.g - rhs.g, 
                             this.b - rhs.b, 
                             this.a - rhs.a);
        }
        return new Color(this.r - rhs, 
                         this.g - rhs, 
                         this.b - rhs, 
                         this.a - rhs);
    }
}
