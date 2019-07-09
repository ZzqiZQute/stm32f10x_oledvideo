///<reference path="./node_modules/@types/node/index.d.ts"/>
///<reference path="./node_modules/@types/sharp/index.d.ts"/>

const { parseTime, sendProgress } = require('./utils');
const { ipcMain, app, dialog } = require('electron');
const path = require('path');
const child_process = require('child_process');
const fs = require('fs');
const sharp = require('sharp');
const rimraf = require('rimraf');
const PRINTJ = require('printj');

sharp.cache(false);

ipcMain.on('getThumbnail', (ev, args) => {
	const ffmpegPath = path.resolve(__dirname, './util/ffmpeg.exe');
	const tempPath = path.resolve(__dirname, './util/temp.jpg');
	child_process.exec(`"${ffmpegPath}" -i "${args}" "${tempPath}" -y`, (err) => {
		const message = err.message;
		const sentences = message.split('\r\n');
		let width, height;
		sentences.forEach((value) => {
			const t = value.trim();
			if (t && t.startsWith('Stream')) {
				const li = t.match(/\d{2,}x\d{2,}/);
				if (li) {
					const m = li[0].split('x');
					width = parseInt(m[0]);
					height = parseInt(m[1]);
					return;
				}
			}
		});
		fs.readFile(tempPath, (err, data) => {
			if (err) {
				console.log(err);
				return;
			}
			let b64 = Buffer.from(data).toString('base64');
			b64 = "url('data:image/jpg;base64," + b64 + "')";
			fs.unlinkSync(tempPath);
			global.mainWindow.webContents.send('onThumbnail', {
				b64,
				width,
				height
			});
		});
	});
});

ipcMain.on('openPathSelect', (ev, videoPath) => {
	const dir = path.dirname(videoPath);
	dialog.showOpenDialog(
		global.mainWindow,
		{
			title: '选择输出文件夹',
			defaultPath: dir ? dir : '',
			properties: [ 'openDirectory' ]
		},
		(pathName) => {
			if (pathName) {
				global.mainWindow.webContents.send('onOutputPath', {
					pathName: pathName[0]
				});
			}
		}
	);
});

ipcMain.on('process', (ev, args) => {
	const { dirPath, videoPath } = args;
	const outputDir = path.resolve(dirPath, './.outTmp');
	const ffmpegPath = path.resolve(__dirname, './util/ffmpeg.exe');
	const generate = () => {
		try {
			fs.mkdirSync(dirPath);
			fs.mkdirSync(outputDir);
		} catch (e) {
			global.mainWindow.webContents.send('cannotMkdir');
			return;
		}
		let time,
			fps,
			predictFrameCnt,
			nowFrame,
			totalReadFrame = 0,
			reading = true,
			currentFrame = 1;
		child_process.exec(`${ffmpegPath} -i "${videoPath}"`, (err, stdout, stderr) => {
			time = parseTime(stderr.match(/Duration:(.*?),/)[1].trim());
			fps = parseFloat(stderr.match(/ (\d+?(.\d+)?) fps,/)[1]);
			predictFrameCnt = Math.ceil(time * fps);
			const fpsBuffer = Buffer.alloc(8);
			const actualFps = fps * 0.9811614512471655;
			fpsBuffer.writeDoubleLE(1000 / actualFps); //STM32 音频误差(22050Hz 误差:1.88%)
			// 1 convert video to image 90%
			const ffmpeg = child_process.spawn(`${ffmpegPath}`, [
				'-i',
				videoPath,
				path.resolve(outputDir, './out_%8d.jpg')
			]);
			//Bug
			// new Promise(async (resolve) => {
			// 	while (reading) {
			// 		while (currentFrame < totalReadFrame) {
			// 			sendProgress(Math.round(90 * currentFrame / predictFrameCnt));
			// 			jpgPath = path.resolve(outputDir, PRINTJ.sprintf('./out_%08d.jpg', i));
			// 			await processImage(jpgPath, videoFile);
			// 			sendProgress(Math.ceil(90 * i / predictFrameCnt));
			// 			rimraf.sync(jpgPath);
			// 			currentFrame++;
			// 		}
			// 	}
			// 	resolve();
			// });
			const child = child_process.fork('./child.js');
			child.send({ dirPath, fps, outputDir, predictFrameCnt });

			ffmpeg.stderr.on('data', async (data) => {
				const msg = data.toString('ascii');
				try {
					totalReadFrame = parseInt(msg.match(/frame=(.*?)fps/)[1].trim());
				} catch (e) {}
				child.send({ reading: true, totalReadFrame });
			});
			ffmpeg.on('close', async (code) => {
				child.send({ reading: false, totalReadFrame });
			});
			child.on('message', (p) => {
				try {
					if (p.msg === 'progress') sendProgress(p.progress);
					else {
						//2 generate mp3 10%
						const ffmpeg_mp3 = child_process.spawn(ffmpegPath, [
							'-i',
							videoPath,
							'-ar',
							'22050',
							path.resolve(dirPath, './audio.wav')
						]);
						ffmpeg_mp3.stderr.on('data', (data) => {
							const temp = data.toString('ascii');
							try {
								const p = parseTime(temp.match(/time=(.*?)bitrate/)[1].trim()) / time;
								sendProgress(Math.ceil(95 + 5 * p));
							} catch (e) {}
						});
						ffmpeg_mp3.on('close', (code) => {
							rimraf.sync(outputDir);
							global.mainWindow.webContents.send('complete');
						});
					}
				} catch (e) {}
			});
		});
	};
	if (fs.existsSync(dirPath)) {
		rimraf.sync(dirPath);
	}
	generate();
});
