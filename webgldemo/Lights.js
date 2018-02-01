class AmbientLS {
    constructor(lowerHemisphereColor, upperHemisphereColor) { this.lowerHemisphereColor = lowerHemisphereColor; this.upperHemisphereColor = upperHemisphereColor; }
}

class OmniSpotLS extends RefFrame {
    constructor(parent, color, coeff0, coeff1, coeff2) { super(parent); this.color = color; this.coeff0 = coeff0; this.coeff1 = coeff1; this.coeff2 = coeff2; }
}

class ParallelLS extends RefFrame {
    constructor(parent, color) { super(parent); this.color = color; }
}

