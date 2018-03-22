import Scene from './Scene';
import { actionTypes } from './Actions';

const initialState = {
    sceneArray: [ new Scene(0, 'hardwood'), new Scene(1, 'biplane'), new Scene(2, 'goku') ],
    curSceneId: 0
};

function updateScene(oldScene, newProps) {
    return Object.assign({}, oldScene, newProps);
}

function jsonSceneLoadAsync(oldScene) {

}

function jsonSceneLoadEnded(oldScene, json) {
    const { id, name } = oldScene;
    const s = new Scene(id, name);
    s.init(json);
    return s;
}

function sceneReducer(state, action) {
    switch (action.type) {
    case actionTypes.JSON_SCENE_LOAD_ASYNC:
        return jsonSceneLoadAsync(state);

    case actionTypes.JSON_SCENE_LOAD_ENDED:
        return jsonSceneLoadEnded(state, action.responseText);

    case actionTypes.CHANGE_CAM_INDEX:
        return updateScene(state, { cameraIdx: action.camIndex });

    case actionTypes.TOGGLE_WIREFRM:
        return updateScene(state, { drawWireFrm: !state.drawWireFrm });

    case actionTypes.TOGGLE_NORMALS:
        return updateScene(state, { drawNormals: !state.drawNormals });
    
    default:
        return state;
    }
}

function sceneArrayReducer(state, action) {
    return state.map((item) => { 
        return (item.id === action.id) ? sceneReducer(item, action) : item; 
    });
}

function curSceneIdReducer(state, action) {
    if (action.type === actionTypes.CHANGE_CUR_SCENE) {
        return action.id;
    }
    return state;
}

export default function appReducer(state = initialState, action) {
    return {
        sceneArray: sceneArrayReducer(state.sceneArray, action),
        curSceneId: curSceneIdReducer(state.curSceneId, action)
    };
}
