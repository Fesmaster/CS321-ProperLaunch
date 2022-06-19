const fs = require('fs');
const { get } = require('http');

let rawdata = fs.readFileSync('../server/serverdata.json');
let data = JSON.parse(rawdata);


function getQuery() {
    var url_string = window.location.href;
    var url = new URL(url_string);
    var usernameParam = url.searchParams.get("q");
    document.title = usernameParam;
    return usernameParam;
}

var container = document.getElementById("launches-container")

let launchesData = data["results"]

for (var launch in launchesData) {

    const myJson = JSON.stringify(launchesData[launch]);

    console.log(myJson);

    if (myJson.toLowerCase().includes(getQuery().toLowerCase())) {
        var itemCard = document.createElement("div");


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
                    <span class="block font-semibold  text-sm">Launchpad Name: ${launchesData[launch]["pad"]["name"] || "Undefined"}</span>
                    <span class="block font-semibold  text-sm">Location: ${launchesData[launch]["pad"]["location"]["name"] || "Undefined"}</span>
                    <span class="block font-semibold  text-sm">Mission Title: ${launchesData[launch]["mission"]["name"] || "Undefined"}</span>

                </div>    
            `
        } catch (error) {
            itemCard.innerHTML = `
                <img src="${launchesData[launch]["image"]}" alt="delta" class="object-scale-down h-15 w-15">
                <div class="m-4">
                    <span class="font-bold">${launchesData[launch]["name"]} <footer></footer></span>
                    <span class="block font-semibold text-sm">Rocket Name: ${launchesData[launch]["rocket"]["configuration"]["name"] || "Undefined"}</span>
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