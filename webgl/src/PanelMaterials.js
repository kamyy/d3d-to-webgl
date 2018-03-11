import React, { Component } from 'react';

export default class PanelMaterials extends Component {
    constructor(props) {
        super(props);

        this.onSceneChange = this.onSceneChange.bind(this);
        this.onSceneLoaded = this.onSceneLoaded.bind(this);
        this.onClickClear = this.onClickClear.bind(this);
        this.onClickList = this.onClickList.bind(this);
        this.onChange = this.onChange.bind(this);
        this.props.onRef(this);

        const currentScene = props.getCurrentScene();
        this.state = { 
            materials: currentScene.filteredMaterials,
            filterTxt: '' 
        };
    }

    onSceneChange() {
        const currentScene = this.props.getCurrentScene();
        currentScene.filterMaterials('');
        this.setState({ 
            materials: currentScene.filteredMaterials,
            filterTxt: ''
        });
    }

    onSceneLoaded(loadedScene) {
        const currentScene = this.props.getCurrentScene();
        if (currentScene === loadedScene) {
            currentScene.filterMaterials('');
            this.setState({ 
                materials: currentScene.filteredMaterials,
                filterTxt: ''
            });
        }
    }

    onChange(event) {
        const currentScene = this.props.getCurrentScene();
        currentScene.filterMaterials(event.target.value);
        this.setState({
            filterTxt: event.target.value
        });
    }
    
    onClickClear(event) {
        const currentScene = this.props.getCurrentScene();
        currentScene.filterMaterials('');
        this.setState({
            filterTxt: ''
        });
    }

    onClickList(event) {
        const currentScene = this.props.getCurrentScene();
        currentScene.filterMaterials(event.target.id);
        this.setState({ 
            filterTxt: event.target.id
        });
    }

    render() {
        const currentScene = this.props.getCurrentScene();

        if (currentScene.rootNode) {
            return <div id='MaterialPanel' className='CanvasPanel'>
                <fieldset className='Fieldset'><legend className='Legend'>Filter By Material</legend>
                    <input  id='FilterTxt' 
                            type='text' 
                            value={this.state.filterTxt} 
                            spellCheck={false} 
                            onChange={this.onChange}
                            className='MaterialInput' 
                            />
                    <button id='Clear' onClick={this.onClickClear}>
                        <img alt='Clear' src='./cross.png' width='12' height='12'/>
                    </button>
                </fieldset>

                <fieldset className='Fieldset'><legend className='Legend'>Materials</legend> {
                    [...currentScene.filteredMaterials].map(m => 
                        <div id={m.name} key={m.name} className='MaterialItem' onClick={this.onClickList}>{m.name}</div>
                    )
                } </fieldset>
            </div>
        }

        return null;
    }
}
