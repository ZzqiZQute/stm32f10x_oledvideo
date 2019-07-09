<template>
  <div class="outer" @dragover="handleDragOver" @drop="handleDrop">
    <div v-if="thumbnail">
      <div class="img" :style="imageStyle"/>
    </div>
    <div :class="['inner',{'file-exist':hasFile}]">{{filePath}}</div>
  </div>
</template>

<script>
const { ipcRenderer } = global.electron;

export default {
  name: "DragArea",
  data() {
    return {
      hasFile: false,
      filePath: "请将视频拖拽到此处",
      filters: ["mp4", "avi", "wmv","mts"],
      thumbnail: false,
      thumbnailPath: "",
      imageStyle: {}
    };
  },
  methods: {
    handleDragOver(ev) {
      ev.preventDefault();
    },
    handleDrop(ev) {
      const temp = ev.dataTransfer.files[0].path;
      if (this.validVideo(temp)) {
        this.hasFile = true;
        this.filePath = temp;
        this.$store.commit('setFilePath',temp)
        ipcRenderer.send("getThumbnail", this.filePath);
      } else {
        this.hasFile = false;
        this.thumbnail = false;
        this.filePath = "无效或不支持的视频路径";
        this.$store.commit('setFilePath','')
      }
    },
    validVideo(path) {
      return this.filters.some(value => {
        return path.toLowerCase().endsWith(value);
      });
    },
    registerOnThumbnail() {
      ipcRenderer.on("onThumbnail", (ev, args) => {
        this.thumbnail = true;
        this.thumbnailPath = args.b64;
        this.imageStyle = {
          "background-image": this.thumbnailPath,
          "background-size": "cover",
          height: "200px",
          width: `${(200 * args.width) / args.height}px`
        };
      });
    }
  },
  mounted() {
    this.registerOnThumbnail()
  }
};
</script>

<style scoped>
.outer {
  width: 70%;
  min-height: 300px;
  border-style: dashed;
  border-radius: 40px;
  display: flex;
  flex-direction: column;
  justify-content: space-evenly;
  align-items: center;
  user-select: none;
  padding: 20px 0;
  border-color: gray
}
.inner {
  text-align: center;
  font-size: 40px;
}
.file-exist {
  font-size: 26px;
}
.img {
  width: 200px;
  height: 200px;
}
</style>
