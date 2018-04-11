// @flow

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
    onSceneLoad(id: number, scene: Object) {
        return { type: actionTypes.onSceneLoad, id, scene };
    },
    changeScene(id: number) {
        return { type: actionTypes.changeScene, id };
    },

    changeCamera(id: number, cameraIdx: number) {
        return { type: actionTypes.changeCamera, id, cameraIdx };
    },
    changeFieldOfView(id: number, fieldOfView: number) {
        return { type: actionTypes.changeFieldOfView, id, fieldOfView };
    },
    changeAspectRatio(id: number, aspectRatio: number) {
        return { type: actionTypes.changeAspectRatio, id, aspectRatio };
    },

    changeLightSource(id: number, lightSource: Array<number>) {
        return { type: actionTypes.changeLightSource, id, lightSource };
    },
    changeR(id: number, value: number) {
        return { type: actionTypes.changeR, id, value };
    },
    changeG(id: number, value: number) {
        return { type: actionTypes.changeG, id, value };
    },
    changeB(id: number, value: number) {
        return { type: actionTypes.changeB, id, value };
    },

    toggleWirefrm(id: number) {
        return { type: actionTypes.toggleWirefrm, id };
    },
    toggleNormals(id: number) {
        return { type: actionTypes.toggleNormals, id };
    },

    changeMaterialFilter(id: number, filter: string) {
        return { type: actionTypes.changeMaterialFilter, id, filter };
    }
});
