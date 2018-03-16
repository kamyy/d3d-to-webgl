import PropTypes from 'prop-types'
import React from 'react'
import './App.css'

export default function PanelScene(props) {
    const { listOfScenes, getCurrentScene, onClick } = props;

    return <div id='ScenePanel' className='CanvasPanel'> {
        listOfScenes.map(scene => {
            if (scene === getCurrentScene()) {
                return <button className='SceneButton' key={scene.name} onClick={onClick} style={{color: 'cyan', fontWeight: 'bold'}}>{scene.name}</button>
            } else {
                return <button className='SceneButton' key={scene.name} onClick={onClick}>{scene.name}</button>
            }
        })
    } </div>
}

PanelScene.propTypes = {
    listOfScenes: PropTypes.array.isRequired,
    getCurrentScene: PropTypes.func.isRequired,
    onClick: PropTypes.func.isRequired
}
