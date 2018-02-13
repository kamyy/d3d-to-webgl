class AmbientLS {
    constructor(lowerHemisphereColor, upperHemisphereColor) { 
        this.lowerHemisphereColor = lowerHemisphereColor; this.upperHemisphereColor = upperHemisphereColor;
    }
}

class OmniDirLS extends RefFrame {
    constructor(parent, node) { 
        super(parent, node); 
        this.color  = node.color;
        this.coeff0 = node.coeff0; 
        this.coeff1 = node.coeff1; 
        this.coeff2 = node.coeff2;
    }
}

