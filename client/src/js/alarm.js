

const closebtn = document.getElementById('closebtn')
closebtn.addEventListener('click', function (event) {
    var window = remote.getCurrentWindow();
    window.close()
})
var alarmSound = new Audio();
alarmSound.src ='../images/alarm.wav';

function setAlarm() {
    var ms = document.getElementById("alarmTime").valueAsNumber;
    var alarm = new Date(ms);
    var alarmTime = new Date(alarm.getUTCFullYear(), alarm.getUTCMonth(), alarm.getUTCDate(), alarm.getUTCHours(), alarm.getUTCMinutes(), alarm.getUTCSeconds());
    var timeDifference = alarmTime.getTime() - (new Date()).getTime();
    if (timeDifference < 0) {
        alert('The time has already passed.');
        return;
    }

    window.opener.activateAlarm(timeDifference);
    alert('Alarm set successfully');
}
function activateAlarm() {
    alarmSound.play();
    alert('Launch time approaching!');
}