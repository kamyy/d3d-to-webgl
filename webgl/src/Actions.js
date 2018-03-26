export const actionTypes = Object.freeze({
    onSceneLoad: 'onSceneLoad',
    changeScene: 'changeScene',
    changeCamera: 'changeCamera',
    changeFieldOfView: 'changeFieldOfView',
    changeAspectRatio: 'changeAspectRatio',
    changeLightSource: 'changeLightSource',
    changeR: 'changeR',
    changeG: 'changeG',
    changeB: 'changeB',
    toggleWirefrm: 'toggleWirefrm',
    toggleNormals: 'toggleNormals',
    changeMaterialFilter: 'changeMaterialFilter',
});

export const actionCreators = Object.freeze({
    onSceneLoad(scene) {
        return { type: actionTypes.onSceneLoad, scene };
    },
    changeScene(id) {
        return { type: actionTypes.changeScene, id };
    },

    changeCamera(id, cameraIdx) {
        return { type: actionTypes.changeCamera, id, cameraIdx };
    },
    changeFieldOfView(id, fieldOfView) {
        return { type: actionTypes.changeFieldOfView, id, fieldOfView };
    },
    changeAspectRatio(id, aspectRatio) {
        return { type: actionTypes.changeAspectRatio, id, aspectRatio };
    },

    changeLightSource(id, lightSource) {
        return { type: actionTypes.changeLightSource, id, lightSource };
    },
    changeR(id, value) {
        return { type: actionTypes.changeR, id, value };
    },
    changeG(id, value) {
        return { type: actionTypes.changeG, id, value };
    },
    changeB(id, value) {
        return { type: actionTypes.changeB, id, value };
    },

    toggleWirefrm(id) {
        return { type: actionTypes.toggleWirefrm, id };
    },
    toggleNormals(id) {
        return { type: actionTypes.toggleNormals, id };
    },

    changeMaterialFilter(id, filter) {
        return { type: actionTypes.changeMaterialFilter, id, filter };
    }
});