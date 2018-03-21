export const actionTypes = Object.freeze({
    PARSE_JSON_SCENE: 'PARSE_JSON_SCENE',
    CHANGE_CUR_SCENE: 'CHANGE_CUR_SCENE',
    CHANGE_CAM_INDEX: 'CHANGE_CAM_INDEX',
    TOGGLE_WIREFRM: 'TOGGLE_WIREFRM',
    TOGGLE_NORMALS: 'TOGGLE_NORMALS',
    CHANGE_R: 'CHANGE_R',
    CHANGE_G: 'CHANGE_G',
    CHANGE_B: 'CHANGE_B',
});

export const actionCreators = Object.freeze({
    parseJsonScene(id, json) {
        return { type: actionTypes.PARSE_JSON_SCENE, id, json };
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
