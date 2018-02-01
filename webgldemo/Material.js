class Material {
    static getMaterial(name) {
        if(!Material.map.has(name)) {
            Material.map.set(name, new Material());
        }
        return Material.map.get(name);
    }

    constructor() {
        this._textures = [null, null, null, null, null, null, null, null];
        this._diff = new Color(0.5, 0.5, 0.5);
        this._spec = new Color(0.5, 0.5, 0.5);
        this._shinyExponent = 1.0;
        this._isTranslucent = false;
    }

    setTexture(stage, texture) { this._textures[stage] = texture; }
    getTexture(stage) { return this._textures[stage]; }

    set diff(color) { this._diff = color; }
    get diff() { return this._diff; }

    set spec(color) { this._spec = color; }
    get spec() { return this._spec; }

    set shinyExponent(exp) { this._shinyExponent = exp; }
    get shinyExponent() { return this._shinyExponent; }

    get isTranslucent() { return this._isTranslucent; }
}

Material.map = new Map();

