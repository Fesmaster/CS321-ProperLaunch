const { app, BrowserWindow } = require('electron')
const path = require('path')

function createWindow() {
  const win = new BrowserWindow({
    width: 800,
    height: 600,
    roundedCorners: true,
    webPreferences: {

      nodeIntegration: true,
      contextIsolation: false,
      devTools: true,
      preload: path.join(__dirname, 'preload.js'),
      protocol: 'file',
      slashes: true
    }
  })

  win.loadFile('src/index.html')
}

app.whenReady().then(() => {
  createWindow()

  app.on('activate', () => {
    if (BrowserWindow.getAllWindows().length === 0) {
      createWindow()
    }
  })
})

app.on('window-all-closed', () => {
  if (process.platform !== 'darwin') {
    app.quit()
  }
})
function setAlarm() {
  var ms = document.getElementById(alarmTime).valueAsNumber;
  var alarm = new Date(ms);
  var alarmTime = new Date(alarm.getUTCFullYear(), alarm.getUTCMonth(), alarm.getUTCDate(), alarm.getUTCHours(), alarm.getUTCminutes(), alarm.getUTCSeconds());
  var timeDifference = alarmTime.getTime() - (new Date()).getTime();
  if (timeDifference < 0) {
    alert('Time already passed');
    return;
  }
  setTimeout(() => activateAlarm(), timeDifference);
  alert('Alarm set successfully');
}
function activateAlarm() {
  alert('It is time!');
}
