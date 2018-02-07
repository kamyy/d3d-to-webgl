g_actorMap = new Map();

class Actor {
    constructor(name) {
        if (g_actorMap.has(name)) {
            return g_actorMap.get(name);
        }
        g_actorMap.set(name, this);

        this.name = name; 
        this.models = [];
    }

    add(model) {
        this.models.push(model);
    }

    draw(noAlpha) {
        this.models.forEach(value => value.draw(noAlpha));
    }

    drawNormals() {
        this.models.forEach(value => value.drawNormals());
    }
}

