import Vue from 'vue'
import App from './App.vue'
import store from './store'
import ElementUI from 'element-ui'
Vue.config.productionTip = false
import 'element-ui/lib/theme-chalk/index.css'
Vue.use(ElementUI)
new Vue({
  render: h => h(App),
  store,

}).$mount('#app')
