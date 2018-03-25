import './index.css';

import React from 'react';
import ReactDOM from 'react-dom';
import { Provider } from 'react-redux';

import App, { reduxStore } from './App';
import registerServiceWorker from './registerServiceWorker';

ReactDOM.render(
    <Provider store={reduxStore}><App/></Provider>, 
    document.getElementById('root')
);

if (module.hot) {
    module.hot.accept();
}

registerServiceWorker();
