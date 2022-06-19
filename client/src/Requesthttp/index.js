const { app, BrowserWindow } = require('electron');

function createWindow () {

	const window = new BrowserWindow({
	
		width: 500,

		height: 500,

		webPreferences: {

			nodeIntegration: true

		}
	});

	window.loadFile('index.html');
}

app.whenReady().then(createWindow);

