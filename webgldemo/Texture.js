const g_textureMap = new Map();

class Texture {
    constructor(url) {
        if (g_textureMap.has(url)) {
            return g_textureMap.get(url);
        }
        g_textureMap.set(url, this);

        this.texture = g_GL.createTexture();
        this.bLoaded = false;

        const image = new Image();
        image.onload = () => {
            g_GL.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);
            g_GL.bindTexture(g_GL.TEXTURE_2D, this.texture);
            g_GL.texImage2D(g_GL.TEXTURE_2D, 0, g_GL.RGBA, g_GL.RGBA, g_GL.UNSIGNED_BYTE, image);
            g_GL.generateMipmap(g_GL.TEXTURE_2D);
            this.bLoaded = true;
        };
        image.src = url;
    }
}
