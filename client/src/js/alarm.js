const electron = require('electron')
const path = require('path')
const remote = electron.remote

const closebtn = document.getElementById('closebtn')
closebtn.addEventListener('click', function(event){
    var window = remote.getCurrentWindow();
    window.close()
})