import Vue from 'vue'
import Vuex from 'vuex'

Vue.use(Vuex)

const store = new Vuex.Store({
  state:{
      filePath:''
  },
  mutations:{
    setFilePath(state,filePath){
        state.filePath = filePath
    }  
  }
})

export default store