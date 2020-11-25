import RefFrame from './RefFrame.js';

export default class OmniDirLS extends RefFrame {
    constructor(parent, node) { 
        super(parent, node); 
        this.color  = node.color;
        this.coeff0 = node.coeff0; 
        this.coeff1 = node.coeff1; 
        this.coeff2 = node.coeff2;
    }
}
