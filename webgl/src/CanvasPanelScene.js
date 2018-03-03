import React from 'react';
import './App.css';

export default function CanvasPanelScene(props) {
    const { listOfScenes, getCurrentScene, onClick } = props;

    return <div id='canvas-panel-scene' className='canvas-panel'> {
        listOfScenes.map(scene => {
            if (scene === getCurrentScene()) {
                return <button className='canvas-panel-scene-button' key={scene.name} onClick={onClick} style={{color: 'cyan'}}>{scene.name}</button>
            } else {
                return <button className='canvas-panel-scene-button' key={scene.name} onClick={onClick}>{scene.name}</button>
            }
        })
    } </div>
}
