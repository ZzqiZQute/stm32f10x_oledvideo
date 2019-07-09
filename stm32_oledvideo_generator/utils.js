
module.exports.parseTime = (t)=>{
    const r = t.split(':')
    const h = parseInt(r[0])
    const m = parseInt(r[1])
    const r2 = r[2].split('.')
    const s = parseInt(r2[0])+parseInt(r2[1])/1000
    return h*3600+m*60+s
}

module.exports.sendProgress=(n)=>{
    global.mainWindow.webContents.send('onProgress',n)
}