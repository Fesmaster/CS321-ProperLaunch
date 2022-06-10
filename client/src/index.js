const recentLaunchTemplate = document.querySelector("[data-user-template]")
const recentLaunchContainer = document.querySelector("[data-launch-cards-container]")
const searchInput = document.querySelector("[data-search]")
const fs = require('fs')

let launch = []
searchInput.addEventListener("input", e => {
  const value = e.target.value.toLowerCase()
  launch.forEach(launches =>
    {
  const isVisible = launches.image.toLowerCase.includes(value) || launches.launchName.toLowerCase.includes(value) || launches.missionTitle.toLowerCase.includes(value) ||launches.launchTime.toLowerCase.includes(value) || launches.launchpadName.toLowerCase.includes(value) || launches.agencyName.toLowerCase.includes(value) || launches.rocketName.toLowerCase.includes(value)
  launches.element.classList.toggle("hide", !isVisible)
})
})
fetch('././server/serverdata.json')
.then(response => response.json())
.then(data => {
  launch = data.map(launches =>{
    const recentLaunch = recentLaunchTemplate.content.cloneNode(true).children[0]
    const image = recentLaunch.querySelector("[data-image]")
    const launchName = recentLaunch.querySelector("[data-launchName]")
    const missionTitle = recentLaunch.querySelector("[data-missionTitle]")
    const launchTime = recentLaunch.querySelector("[data-launchTime]")
    const launchpadName = recentLaunch.querySelector("[data-launchpadName]")
    const agencyName = recentLaunch.querySelector("[data-agancyName]")
    const rocketName = recentLaunch.querySelector("[data-rocketName]")
    image.textContent = launches.image
    launchName.textContent = launches.name
    missionTitle.textContent = launches.mission.name
    launchTime.textContent = launches.window_start
    launchpadName.textContent = launches.pad.name
    agencyName.textContent = launches.launch_service_provider
    rocketName.textContent = launches.rocket.configuration.name
    recentLaunchContainer.append(recentLaunch)
    return{launchName: launches.name, missionTitle: launches.mission.name,launchTime: launches.window_start,launchpadName: launches.pad.name,agencyName: launches.launch_service_provider,rockertName: launches.rocket.configuration.name}
  })
})