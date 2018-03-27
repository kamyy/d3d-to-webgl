import './index.css';

import React from 'react';
import ReactDOM from 'react-dom';
import { Provider } from 'react-redux';

import ConnectedApp, { reduxStore } from './App';
import registerServiceWorker from './registerServiceWorker';

ReactDOM.render(
    <Provider store={reduxStore}><ConnectedApp/></Provider>, 
    document.getElementById('root')
);

if (module.hot) {
    module.hot.accept();
}

registerServiceWorker();
