import React from 'react';
import './App.css';

export default function PanelScene(props) {
    const { listOfScenes, getCurrentScene, onClick } = props;

    return <div id='scene-choice' className='canvas-panel'> {
        listOfScenes.map(scene => {
            if (scene === getCurrentScene()) {
                return <button className='scene-choice-button' key={scene.name} onClick={onClick} style={{color: 'cyan'}}>{scene.name}</button>
            } else {
                return <button className='scene-choice-button' key={scene.name} onClick={onClick}>{scene.name}</button>
            }
        })
    } </div>
}
