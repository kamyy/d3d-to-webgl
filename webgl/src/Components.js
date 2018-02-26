import React from 'react';

export function SelectComponent(props) {
    const {
        id,
        options, 
        onChange,
        defaultIndex
    } = props;

    return (
        <select id={id} 
                defaultValue={defaultIndex}
                onChange={ event => onChange(parseInt(event.target.value, 10)) }> 
            { 
                options.map( (opt, idx) => {
                    return <option key={idx} value={idx}> {opt} </option> 
                })
            } 
        </select> 
    )
}

export function CheckboxComponent(props) {
    const {
        id,
        onChange
    } = props;

    return (
        <input id={id}
               key={id}
               type='checkbox'
               onChange={event => onChange(event.target.checked) }>
        </input>
    )
}
