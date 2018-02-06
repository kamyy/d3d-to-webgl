const g_materialMap = new Map();

class Material {
    constructor(name) {
        if (g_materialMap.has(name)) {
            return g_materialMap.get(name);
        }
        g_materialMap.set(name, this);

        this.textures = [null, null, null, null, null, null, null, null];
        this.diff = new Color(0.5, 0.5, 0.5);
        this.spec = new Color(0.5, 0.5, 0.5);
        this.shinyExponent = 1.0;
        this.isTranslucent = false;
    }
}

