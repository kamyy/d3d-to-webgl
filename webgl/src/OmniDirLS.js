// @flow

import RefFrame from './RefFrame.js';

export default class OmniDirLS extends RefFrame {
    color:  Array<number>;
    coeff0: number;
    coeff1: number;
    coeff2: number;

    constructor(parent: RefFrame, node: Object) { 
        super(parent, node); 
        this.color  = node.color;
        this.coeff0 = node.coeff0; 
        this.coeff1 = node.coeff1; 
        this.coeff2 = node.coeff2;
    }
}
