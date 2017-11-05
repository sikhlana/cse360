import helpers from './helpers';

/**
 * First we will load all of this project's JavaScript dependencies which
 * includes Vue and other libraries. It is a great starting point when
 * building robust, powerful web applications using Vue and Laravel.
 */

require('./bootstrap');
require('chart.js');

window.Vue = require('vue');

import VueCircleSlider from 'vue-circle-slider';
require('hchs-vue-charts');

Vue.use(VueCircleSlider);
Vue.use(VueCharts);

/**
 * Next, we will create a fresh Vue application instance and attach it to
 * the page. Then, you may begin adding components to this application
 * or customize the JavaScript scaffolding to fit your unique needs.
 */

Vue.component('container', require('./components/Container.vue'));

window.app = new Vue({
    el: '#app',
    data: {
        title: '',
        user: {},
    },
    watch: {
        title(title)
        {
            helpers.setTitle(title);
        }
    },
    updated()
    {
        helpers.activate(this.$el);
    },
});