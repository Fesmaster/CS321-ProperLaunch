

const closebtn = document.getElementById('closebtn')
closebtn.addEventListener('click', function (event) {
    var window = remote.getCurrentWindow();
    window.close()
})


function setAlarm() {
    var ms = document.getElementById("alarmTime").valueAsNumber;
    var alarm = new Date(ms);
    var alarmTime = new Date(alarm.getUTCFullYear(), alarm.getUTCMonth(), alarm.getUTCDate(), alarm.getUTCHours(), alarm.getUTCMinutes(), alarm.getUTCSeconds());
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