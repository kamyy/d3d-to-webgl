export const actionTypes = Object.freeze({
    JSON_SCENE_LOAD_ASYNC: 'JSON_SCENE_LOAD_ASYNC',
    JSON_SCENE_LOAD_ENDED: 'JSON_SCENE_LOAD_ENDED',
    CHANGE_CUR_SCENE: 'CHANGE_CUR_SCENE',
    CHANGE_CAM_INDEX: 'CHANGE_CAM_INDEX',
    TOGGLE_WIREFRM: 'TOGGLE_WIREFRM',
    TOGGLE_NORMALS: 'TOGGLE_NORMALS',
    CHANGE_R: 'CHANGE_R',
    CHANGE_G: 'CHANGE_G',
    CHANGE_B: 'CHANGE_B',
});

const actionCreators = Object.freeze({
    JsonSceneLoadAsync(id) {
        return { type: actionTypes.JSON_SCENE_LOAD_ASYNC, id };
    },

    JsonSceneLoadEnded(id, json) {
        return { type: actionTypes.JSON_SCENE_LOAD_ENDED, id, json };
    },

    changeCurScene(id) {
        return { type: actionTypes.CHANGE_CUR_SCENE, id };
    },

    changeCamIndex(id, camIndex) {
        return { type: actionTypes.CHANGE_CAM_INDEX, id, camIndex };
    },

    toggleWirefrm(id) {
        return { type: actionTypes.TOGGLE_WIREFRM, id };
    },

    toggleNormals(id) {
        return { type: actionTypes.TOGGLE_NORMALS, id };
    },

    changeR(id, LS, r) {
        return { type: actionTypes.CHANGE_R, id, LS, r };
    },

    changeG(id, LS, g) {
        return { type: actionTypes.CHANGE_G, id, LS, g };
    },

    changeB(id, LS, b) {
        return { type: actionTypes.CHANGE_B, id, LS, b };
    },
});

export default actionCreators;