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

        itemCard.onclick = () => {
            window.location = launchesData[launch]["url"];
        }

        itemCard.className = "bg-white rounded overflow-hidden shadow-md";

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

            </div>    
        `
        container.append(itemCard);
    }

    //console.log(launchesData[launch]["name"].toLowerCase(), getQuery().toLowerCase())

}