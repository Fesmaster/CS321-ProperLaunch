const electron = window.require("electron").remote;
const remote = remote.require('remote');



const button = document.getElementById('alarmClock')

button.addEventListener('click', function(event){
  const modalPath = path.join(__dirname, 'alarm.html')
  let win = new BrowserWindow({ width: 400, height: 200})
  win.on('close', function() { win = null})
  win.loadURL(modalPath)
  win.show()
})
