<template>
  <div id="app">
    <div style="font-size:40px">STM32 12864OLED视频生成器 <small>（wm8978作音频驱动）</small></div>
    <div class="drag-area-container">
      <DragArea/>
    </div>
    <div class="output-path-container">
      <input class="output-path" v-model="selectedPath" placeholder="请选择生成路径" disabled="true">
      <button class="output-path-select" @click="handlePathSelect">...</button>
    </div>
    <div class="generate-button" :style="generateButtonStyle" @click="handleGenerate">
      <div class="generate-button-hint" :style="generateButtonHintStyle">生成</div>

      <div class="generate-button-mask" :style="generateButtonMaskStyle"></div>
    </div>
  </div>
</template>

<script>
import DragArea from "./components/DragArea";
import Vue from "vue";
import { setTimeout } from "timers";
const { ipcRenderer } = global.electron;
export default {
  name: "app",
  components: {
    DragArea
  },
  methods: {
    handlePathSelect() {
      ipcRenderer.send("openPathSelect", this.$store.state.filePath);
    },
    handleGenerate() {
      if (this.canGenerate) {
        this.canGenerate = false;
        const { filePath } = this.$store.state;
        const selectedPath = this.selectedPath;
        if (!selectedPath || !filePath) {
          this.$message({
            type: "error",
            message: "视频路径或输出路径错误，请检查"
          });
          this.canGenerate = true;
          return;
        }

        const dirPath = selectedPath;
        const videoPath = filePath;
        // const dirPath = "D:\\cxknmsl";
        // const videoPath =
        //   "D:\\万恶之源，蔡徐坤打篮球视频流出! - 1.万恶之源【蔡徐坤打篮球】(Av47730455,P1).mp4";
        Vue.set(this.generateButtonStyle, "width", "60%");
        Vue.set(this.generateButtonStyle, "height", "15px");
        Vue.set(this.generateButtonStyle, "border-radius", "0");
        Vue.set(this.generateButtonStyle, "cursor", "initial");
        Vue.set(this.generateButtonStyle, "box-shadow", "0 0 0");
        Vue.set(this.generateButtonStyle, "background-color", "white");
        Vue.set(this.generateButtonHintStyle, "opacity", 0);
        setTimeout(() => {
          ipcRenderer.send("process", {
            dirPath,
            videoPath
          });
          // const t = setInterval(() => {
          //   width = parseInt(this.generateButtonMaskStyle.width.split("%")[0]);
          //   Vue.set(this.generateButtonMaskStyle, "width", `${width + 1}%`);
          //   if (width > 100) {
          //     clearInterval(t);
          //     this.$message({
          //       type: "success",
          //       message: "生成成功"
          //     });
          //     Vue.set(this.generateButtonMaskStyle, "width", `0%`);
          //     Vue.delete(this.generateButtonStyle, "width");
          //     Vue.delete(this.generateButtonStyle, "border-radius");
          //     Vue.delete(this.generateButtonStyle, "height");
          //     Vue.delete(this.generateButtonStyle, "cursor");
          //     Vue.delete(this.generateButtonStyle, "margin-top");
          //     Vue.delete(this.generateButtonStyle, "box-shadow");
          //     Vue.delete(this.generateButtonStyle, "background-color");
          //     Vue.delete(this.generateButtonHintStyle, "opacity");
          //     this.canGenerate = true;
          //   }
          // }, 16.6666667);
        }, 400);
      }
    },
    registerMain() {
      ipcRenderer.on("onOutputPath", (ev, arg) => {
        const { pathName } = arg;
        this.selectedPath = pathName;
      });
      ipcRenderer.on("onProgress", (ev, arg) => {
        Vue.set(this.generateButtonMaskStyle, "width", `${arg}%`);
      });
      ipcRenderer.on("complete", () => {
        this.$message({
          type: "success",
          message: "生成成功"
        });
        this.recover()
      });
      ipcRenderer.on('cannotMkdir',()=>{
        this.$message({
          type: "error",
          message: "不知道为什么，这个文件夹正在被占用，请检查一下"
        })
        this.recover()
      })
    },
    recover(){
        Vue.set(this.generateButtonMaskStyle, "width", `0%`);
        Vue.delete(this.generateButtonStyle, "width");
        Vue.delete(this.generateButtonStyle, "border-radius");
        Vue.delete(this.generateButtonStyle, "height");
        Vue.delete(this.generateButtonStyle, "cursor");
        Vue.delete(this.generateButtonStyle, "box-shadow");
        Vue.delete(this.generateButtonStyle, "background-color");
        Vue.delete(this.generateButtonHintStyle, "opacity");
        this.canGenerate = true;
    }
  },
  data() {
    return {
      selectedPath: "",
      generateButtonStyle: {},
      generateButtonHintStyle: {},
      generateButtonMaskStyle: {
        width: "0%"
      },
      canGenerate: true
    };
  },
  mounted() {
    this.registerMain();
  }
};
</script>

<style>
#app {
  font-family: 微软雅黑, "Avenir", Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 40px;
}
.drag-area-container {
  margin-top:30px;
  display: flex;
  justify-content: center;
  align-items: center;
}
.output-path-container {
  margin: 10px auto;
  width: 70%;
  box-sizing: border-box;
  display: flex;
  align-items: center;
  padding: 10px;
}
.output-path {
  font-size: 24px;
  min-width: 0;
  flex: 1;
  padding: 10px;
  font-family: 微软雅黑;
}
.output-path:disabled {
  background-color: white;
  border: none;
}
.output-path-select {
  font-size: 24px;
  width: 60px;
  padding: 10px;
}
.output-path::placeholder {
  font-family: 微软雅黑;
}
.generate-button {
  box-shadow: 0 0 7px;
  background-color: #2196f3;
  border: darkgray 1px solid;
  font-size: 20px;
  padding: 10px 50px;
  font-family: 微软雅黑;
  transition: all 400ms;
  display: inline-block;
  user-select: none;
  cursor: pointer;
  height: 40px;
  position: relative;
  width: 3%;
  overflow: hidden;
  border-radius: 20px;
}
.generate-button:hover {
  background-color: #008ed4;
}
.generate-button:active {
  box-shadow: 0 0 2px;
}
.generate-button-hint {
  z-index: 99;
  position: absolute;
  left: 0;
  top: 0;
  width: 100%;
  height: 100%;
  display: flex;
  align-items: center;
  justify-content: center;
  opacity: 1;
  transition: all 400ms;
}
.generate-button-hint:hover {
  color: white;
}
.generate-button-mask {
  position: absolute;
  left: 0;
  top: 0;
  width: 0%;
  height: 100%;
  background-color: lime;
  transition: width 100ms;
}
.el-message {
  font-family: 微软雅黑;
}

button:focus {
  outline: none;
}
* {
  font-family: 微软雅黑;
}
</style>
