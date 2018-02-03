class Material {
    static get(name) {
        return Material.map.get(name);
    }

    constructor(name) {
        if (Material.map.has(name)) {
            return Material.map.get(name);
        }
        Material.map.set(name, this);

        this.textures = [null, null, null, null, null, null, null, null];
        this.diff = new Color(0.5, 0.5, 0.5);
        this.spec = new Color(0.5, 0.5, 0.5);
        this.shinyExponent = 1.0;
        this.isTranslucent = false;
    }
}

Material.map = new Map();

