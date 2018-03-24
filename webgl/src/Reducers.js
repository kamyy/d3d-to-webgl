import Scene from './Scene';
import { actionTypes } from './Actions';

// sceneArray: [ new Scene(0, 'hardwood'), new Scene(1, 'biplane'), new Scene(2, 'goku') ],

function onSceneLoad(scene) {
    const { id, cameras, cameraIdx, currentLS, omniDirLS, ambientLS, drawWirefrm, drawNormals } = scene;

    return {
        id: id,
        cameras: cameras.map(cam => ( { fieldOfView: cam.fieldOfView, aspectRatio: cam.aspectRatio } )),
        cameraIdx: cameraIdx,
        currentLS: currentLS,
        omniDirLS: [...omniDirLS.color],
        lowerAmbientLS: [...ambientLS.lowerHemisphereColor],
        upperAmbientLS: [...ambientLS.upperHemisphereColor],
        drawWirefrm: drawWirefrm,
        drawNormals: drawNormals,
    }
}

function changeFieldOfView(state, fieldOfView) {
    return state.cameras.map((c, i) => (state.cameraIdx === i) ? { fieldOfView: fieldOfView, aspectRatio: c.aspectRatio } : c);
}

function changeAspectRatio(state, aspectRatio) {
    return state.cameras.map((c, i) => (state.cameraIdx === i) ? { fieldOfView: c.fieldOfView, aspectRatio: aspectRatio } : c);
}

function changeColor(state, value, idx) {
    switch (state.currentLS) {
    case 'Omni Directional':
        return { omniDirLS: state.omniDirLS.map((v, i) => (idx === i) ? value : v) };
    case 'Lower Ambient':
        return { lowerAmbientLS: state.lowerAmbientLS.map((v, i) => (idx === i) ? value : v) };
    case 'Upper Ambient':
        return { upperAmbientLS: state.upperAmbientLS.map((v, i) => (idx === i) ? value : v) };
    default:
        return state;
    }
}

function sceneReducer(state, action) {
    switch (action.type) {
    case actionTypes.onSceneLoad:
        return onSceneLoad(action.scene);

    case actionTypes.changeCamera:
        return Object.assign({}, state, { cameraIdx: action.cameraIdx });
    case actionTypes.changeFieldOfView:
        return Object.assign({}, state, { cameras: changeFieldOfView(state, action.fieldOfView) });
    case actionTypes.changeAspectRatio:
        return Object.assign({}, state, { cameras: changeAspectRatio(state, action.aspectRatio) });

    case actionTypes.changeLightSource:
        return Object.assign({}, state, { currentLS: action.lightSource });
    case actionTypes.changeR:
        return Object.assign({}, state, changeColor(state, action.value, 0));
    case actionTypes.changeG:
        return Object.assign({}, state, changeColor(state, action.value, 1));
    case actionTypes.changeB:
        return Object.assign({}, state, changeColor(state, action.value, 2));

    case actionTypes.toggleWirefrm:
        return Object.assign({}, state, { drawWireFrm: !state.drawWireFrm });
    case actionTypes.toggleNormals:
        return Object.assign({}, state, { drawNormals: !state.drawNormals });
    
    default:
        return state;
    }
}

function sceneArrayReducer(state, action) {
    return state.map((scene, id) => (id === action.id) ? sceneReducer(scene, action) : scene);
}

function curSceneIdReducer(state, action) {
    if (action.type === actionTypes.changeScene) {
        return action.id;
    }
    return state;
}

const initialState = {
    sceneArray: [ null, null, null ],
    curSceneId: 0,
};

export default function appReducer(state = initialState, action) {
    return {
        sceneArray: sceneArrayReducer(state.sceneArray, action),
        curSceneId: curSceneIdReducer(state.curSceneId, action)
    };
}
