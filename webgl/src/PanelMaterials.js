import React, { Component } from 'react';

export default class PanelMaterials extends Component {
    constructor(props) {
        super(props);

        const currentScene = props.getCurrentScene();
        const materials = Array.from(currentScene.mapOfMaterials.values());
        const filterTxt = '';

        this.state = { materials, filterTxt };
        this.onSceneChange = this.onSceneChange.bind(this);
        this.onSceneLoaded = this.onSceneLoaded.bind(this);
        this.onChange = this.onChange.bind(this);
        this.props.onRef(this);

        currentScene.filteredMaterials = [];
    }

    filterMaterials(text) {
        const currentScene = this.props.getCurrentScene();
        const materialList = Array.from(currentScene.mapOfMaterials.values());
        text = text.toLowerCase();

        return materialList.filter(m => m.name.toLowerCase().includes(text));
    }

    onSceneChange() {
        const currentScene = this.props.getCurrentScene();
        const materials = Array.from(currentScene.mapOfMaterials.values());
        const filterTxt = '';
        this.setState({ 
            materials, 
            filterTxt 
        });
        currentScene.filteredMaterials = materials;
    }

    onSceneLoaded(loadedScene) {
        const currentScene = this.props.getCurrentScene();
        if (currentScene === loadedScene) {
            const materials = Array.from(currentScene.mapOfMaterials.values());
            const filterTxt = '';
            this.setState({ 
                materials, 
                filterTxt 
            });
            currentScene.filteredMaterials = materials;
        }

    }

    onChange(event) {
        const currentScene = this.props.getCurrentScene();
        const filterTxt = event.target.value;
        this.setState({filterTxt});

        currentScene.filteredMaterials = this.filterMaterials(filterTxt);
    }

    render() {
        const { filterTxt } = this.state;

        return <div id='MaterialPanel' className='CanvasPanel'>
            <fieldset className='Fieldset'> <legend className='Legend'>Filter By Material</legend>
                <input id='filterTxt' className='MaterialInput' type='text' spellCheck={false} onChange={this.onChange}/>
            </fieldset>
            <fieldset className='Fieldset'><legend className='Legend'>Materials</legend> {
                this.filterMaterials(filterTxt).map(m => <div key={m.name} className='MaterialItem'>{m.name}</div>
                )
            } </fieldset>
        </div>
    }
}
