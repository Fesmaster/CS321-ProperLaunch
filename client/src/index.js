const button = document.getElementById('alarm');
button.addEventListener('click', () => {
  createBrowserWindow();
});

function createBrowserWindow() {
  window.open("alarm.html", "_blank", "width=250,height=200");
}