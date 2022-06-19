const fs = require('fs');
const { get } = require('http');

let rawdata = fs.readFileSync('../server/serverdata.json');
let data = JSON.parse(rawdata);


function changeOrder(chosen) {
    var container = document.getElementById("launches-container");

    container.innerHTML = ""

    let launchesData = data["results"];

    if(chosen == "ascending") {
        for (var launch in launchesData) {

            const myJson = JSON.stringify(launchesData[launch]);

            console.log(launchesData[launch]['net']);

            var itemCard = document.createElement("div");



            var alarm = new Date(launchesData[launch]["net"]);
            var alarmTime = new Date(alarm.getUTCFullYear(), alarm.getUTCMonth(), alarm.getUTCDate(), alarm.getUTCHours(), alarm.getUTCMinutes(), alarm.getUTCSeconds());
            var timeDifference = alarmTime.getTime() - (new Date()).getTime();
            if (timeDifference > 0) {
                //alert('Time already passed');


                

                itemCard.className = "bg-white rounded overflow-hidden shadow-md";

                try {
                    itemCard.innerHTML = `
                        <img src="${launchesData[launch]["image"]}" alt="delta" class="object-scale-down h-15 w-15">
                        <div class="m-4">
                            <span class="font-bold">${launchesData[launch]["name"]} <footer></footer></span>
                            <span class="block font-semibold  text-sm">Rocket Name: ${launchesData[launch]["rocket"]["configuration"]["name"] || "Undefined"}</span>
                            <span class="block font-semibold  text-sm">Start Date: ${launchesData[launch]["window_start"] || "Undefined"}</span>
                            <span class="block font-semibold  text-sm">End Date: ${launchesData[launch]["window_end"] || "Undefined"}</span>
                            <span class="block font-semibold  text-sm">Agency Name: ${launchesData[launch]["launch_service_provider"]["name"] || "Undefined"}</span>
                            <span class="block font-semibold text-sm">Launchpad Name: ${launchesData[launch]["pad"]["name"] || "Undefined"}</span>
                            <span class="block font-semibold  text-sm">Location: ${launchesData[launch]["pad"]["location"]["name"] || "Undefined"}</span>
                            <span class="block font-semibold  text-sm">Mission Title: ${launchesData[launch]["mission"]["name"] || "Undefined"}</span>

                        </div>    
                    `
                } catch (error) {
                    itemCard.innerHTML = `
                        <img src="${launchesData[launch]["image"]}" alt="delta" class="object-scale-down h-15 w-15">
                        <div class="m-4">
                            <span class="font-bold">${launchesData[launch]["name"]} <footer></footer></span>
                            <span class="block font-semibold  text-sm">Rocket Name: ${launchesData[launch]["rocket"]["configuration"]["name"] || "Undefined"}</span>
                            <span class="block font-semibold  text-sm">Start Date: ${launchesData[launch]["window_start"] || "Undefined"}</span>
                            <span class="block font-semibold  text-sm">End Date: ${launchesData[launch]["window_end"] || "Undefined"}</span>
                            <span class="block font-semibold  text-sm">Agency Name: ${launchesData[launch]["launch_service_provider"]["name"] || "Undefined"}</span>
                            <span class="block font-semibold  text-sm">Launchpad Name: ${launchesData[launch]["pad"]["name"] || "Undefined"}</span>
                            <span class="block font-semibold  text-sm">Location: ${launchesData[launch]["pad"]["location"]["name"] || "Undefined"}</span>
                            <span class="block font-semibold  text-sm">Mission Title: Unknown</span>

                        </div>    
                    `
                }
                container.append(itemCard);

            }


            //console.log(launchesData[launch]["name"].toLowerCase(), getQuery().toLowerCase())

        }
    }
    else {
        for (var launch in launchesData.reverse()) {

            const myJson = JSON.stringify(launchesData[launch]);

            console.log(launchesData[launch]['net']);

            var itemCard = document.createElement("div");



            var alarm = new Date(launchesData[launch]["net"]);
            var alarmTime = new Date(alarm.getUTCFullYear(), alarm.getUTCMonth(), alarm.getUTCDate(), alarm.getUTCHours(), alarm.getUTCMinutes(), alarm.getUTCSeconds());
            var timeDifference = alarmTime.getTime() - (new Date()).getTime();
            if (timeDifference > 0) {
                //alert('Time already passed');


                

                itemCard.className = "bg-white rounded overflow-hidden shadow-md";


                try {
                    itemCard.innerHTML = `
                        <img src="${launchesData[launch]["image"]}" alt="delta" class="object-scale-down h-15 w-15">
                        <div class="m-4">
                            <span class="font-bold">${launchesData[launch]["name"]} <footer></footer></span>
                            <span class="block text-gray-500 text-sm">Rocket Name: ${launchesData[launch]["rocket"]["configuration"]["name"] || "Undefined"}</span>
                            <span class="block text-gray-500 text-sm">Start Date: ${launchesData[launch]["window_start"] || "Undefined"}</span>
                            <span class="block text-gray-500 text-sm">End Date: ${launchesData[launch]["window_end"] || "Undefined"}</span>
                            <span class="block text-gray-500 text-sm">Agency: ${launchesData[launch]["launch_service_provider"]["name"] || "Undefined"}</span>
                            <span class="block text-gray-500 text-sm">Launchpad: ${launchesData[launch]["pad"]["name"] || "Undefined"}</span>
                            <span class="block text-gray-500 text-sm">Location: ${launchesData[launch]["pad"]["location"]["name"] || "Undefined"}</span>
                            <span class="block text-gray-500 text-sm">Mission: ${launchesData[launch]["mission"]["name"] || "Undefined"}</span>

                        </div>    
                    `
                } catch (error) {
                    itemCard.innerHTML = `
                        <img src="${launchesData[launch]["image"]}" alt="delta" class="object-scale-down h-15 w-15">
                        <div class="m-4">
                            <span class="font-bold">${launchesData[launch]["name"]} <footer></footer></span>
                            <span class="block text-gray-500 text-sm">Rocket Name: ${launchesData[launch]["rocket"]["configuration"]["name"] || "Undefined"}</span>
                            <span class="block text-gray-500 text-sm">Start Date: ${launchesData[launch]["window_start"] || "Undefined"}</span>
                            <span class="block text-gray-500 text-sm">End Date: ${launchesData[launch]["window_end"] || "Undefined"}</span>
                            <span class="block text-gray-500 text-sm">Agency: ${launchesData[launch]["launch_service_provider"]["name"] || "Undefined"}</span>
                            <span class="block text-gray-500 text-sm">Launchpad: ${launchesData[launch]["pad"]["name"] || "Undefined"}</span>
                            <span class="block text-gray-500 text-sm">Location: ${launchesData[launch]["pad"]["location"]["name"] || "Undefined"}</span>
                            <span class="block text-gray-500 text-sm">Mission: Unknown</span>

                        </div>    
                    `
                }
                    
                container.append(itemCard);

            }


            //console.log(launchesData[launch]["name"].toLowerCase(), getQuery().toLowerCase())

        } 
    }
}

window.onload = () => {
    changeOrder("ascending");
}