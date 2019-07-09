const sharp = require('sharp');
const fs = require('fs');
const path = require('path');
const rimraf = require('rimraf');
const PRINTJ = require('printj');
const { EventEmitter } = require('events');
const emitter = new EventEmitter();
sharp.cache(false);
let videoFile = '',
	outputDir = '',
	reading = true,
	totalReadFrame = 0,
	currentFrame = 1,
	predictFrameCnt = 0,
	jpgPath = '',
	dirPath = '',
	canRead = true;
function processImage(jpgPath, videoFile) {
	return new Promise((resolve) => {
		sharp(jpgPath).resize({ width: 128, height: 64, fit: 'contain' }).grayscale().raw().toBuffer().then((data) => {
			for (let i = 0; i < 128; i++) {
				for (let j = 0; j < 8; j++) {
					let d = 0;
					for (let k = 0; k < 8; k++) {
						if (data[(j * 8 + k) * 128 + i] > 128) {
							d |= 1 << k;
						}
					}
					fs.writeSync(videoFile, Buffer.from([ d ]));
				}
			}
			resolve();
		});
	});
}
emitter.on('over', async () => {
	if (reading === false) {
		while (currentFrame <= totalReadFrame) {
			jpgPath = path.resolve(outputDir, PRINTJ.sprintf('./out_%08d.jpg', currentFrame));
			await processImage(jpgPath, videoFile);
			process.send({ msg: 'progress', progress: Math.ceil(95 * currentFrame / predictFrameCnt) });
			rimraf.sync(jpgPath);
			currentFrame++;
		}
		fs.closeSync(videoFile);
		process.send({ msg: 'complete' });
		process.exit();
	}
});
async function proc() {
	canRead = false;
	while (currentFrame <= totalReadFrame) {
		jpgPath = path.resolve(outputDir, PRINTJ.sprintf('./out_%08d.jpg', currentFrame));
		await processImage(jpgPath, videoFile);
		try {
			process.send({ msg: 'progress', progress: Math.ceil(95 * currentFrame / predictFrameCnt) });
		} catch (e) {}
		rimraf.sync(jpgPath);
		currentFrame++;
	}
	canRead = true;
	emitter.emit('over');
}
process.on('message', (msg) => {
	if ((msg.fpsBuffer && msg.outputDir && msg.dirPath, msg.predictFrameCnt)) {
		outputDir = msg.outputDir;
		dirPath = msg.dirPath;
		predictFrameCnt = msg.predictFrameCnt;
		videoFile = fs.openSync(path.resolve(dirPath, './video.bin'), 'w');
		const fpsBuffer = Buffer.alloc(8);
		const actualFps = msg.fps * 0.9811614512471655;
		fpsBuffer.writeDoubleLE(1000 / actualFps); //STM32 音频误差(22050Hz 误差:1.88%)
		fs.writeSync(videoFile, fpsBuffer);
	} else {
		if (msg.totalReadFrame && msg.reading !== undefined) {
			reading = msg.reading;
			totalReadFrame = msg.totalReadFrame;
			if (reading) {
				if (canRead) proc();
			}
		}
	}
});
