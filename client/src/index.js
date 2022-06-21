const button = document.getElementById('alarm');
button.addEventListener('click', () => {
  createBrowserWindow();
});

function createBrowserWindow() {
  window.open("alarm.html", "_blank", "width=300,height=200");
}

function activateAlarm(timeDifference) {
 
  setTimeout(() => {
    document.getElementById('alarmId').play();  
    return;
  }, timeDifference);
  setTimeout(() => {
    alert('Launching time approaching!');
  }, timeDifference);
}