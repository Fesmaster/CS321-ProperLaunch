
function httpGet(url) {
    let xmlHttpReq = new XMLHttpRequest();
    xmlHttpReq.open("GET", url, false);
    xmlHttpReq.send(null);
    return xmlHttpReq.responseText;
}
var data = "";
function formatDate(date) {
    var d = new Date(date),
        month = '' + (d.getMonth() + 1),
        day = '' + d.getDate(),
        year = d.getFullYear();

    if (month.length < 2) 
        month = '0' + month;
    if (day.length < 2) 
        day = '0' + day;

    return [year, month, day].join('-');
}
function renderGrid(url, reverse, containerId, isForUpcoming) {
    var jsonRawData = httpGet(url);
    jsonRawData = jsonRawData.slice(0, -1);
    data = jsonRawData.replace(/(^"|"$)/g, "'");
    data = JSON.parse(data);
    populateContainer(reverse, containerId, isForUpcoming);
}

function populateContainer(reverse, containerId, isForUpcoming) {
    document.getElementById(containerId).innerHTML = "";

    var container = document.getElementById(containerId)

    let launchesData = data["results"]
    let currentDate = new Date();
    currentDate.setDate(currentDate.getDate() - 2);
    if (reverse) {
        launchesData.reverse();
    }
    for (var launch in launchesData) {
        let windowStart = Date.parse(launchesData[launch].status.window_start);
        if (isForUpcoming && Date.parse(currentDate) > windowStart) {
         continue;
        }

        //if (myJson.toLowerCase().includes(getQuery().toLowerCase())) {
        var itemCard = document.createElement("div");

        var startDate = launchesData[launch]["status"]["window_start"];
        startDate = formatDate(startDate);
        var endDate = launchesData[launch]["status"]["window_start"];
        endDate = formatDate(endDate);
        itemCard.className = "bg-white rounded overflow-hidden shadow-md";

        try {
            itemCard.innerHTML = `
                <div class="flex flex-col">
                    <img src="${launchesData[launch]["image_url"]}" alt="delta" class="object-scale-down h-15 w-15">
                    <div class="m-4 text-xs text-center flex flex-col">
                        <span>${launchesData[launch]["name"]}</span>
                        <a class="underline text-blue-600" id="launchTitle${launch}" href="javascript:displayDetails('${launch}')">View Details</a>
                    </div>
                    <div id="launchDetail${launch}" class="hidden m-4">
                        <span class="block font-semibold  text-sm">Rocket Name: ${launchesData[launch]["rocket"]["name"] || "Undefined"}</span>
                        <span class="block font-semibold  text-sm">Start Date: ${startDate || "Undefined"}</span>
                        <span class="block font-semibold  text-sm">End Date: ${ endDate|| "Undefined"}</span>
                        <span class="block font-semibold  text-sm">Agency Name: ${launchesData[launch]["provider"]["name"] || "Undefined"}</span>
                        <span class="block font-semibold  text-sm">Launchpad Name: ${launchesData[launch]["pad"]["name"] || "Undefined"}</span>
                        <span class="block font-semibold  text-sm">Location: ${launchesData[launch]["pad"]["location"]["name"] || "Undefined"}</span>
                        <span class="block font-semibold  text-sm">Mission Title: ${launchesData[launch]["mission"]["name"] || "Undefined"}</span>
                    </div>
                </div>
            `
        } catch (error) {
            itemCard.innerHTML = `
                <img src="${launchesData[launch]["image_url"]}" alt="delta" class="object-scale-down h-15 w-15">
                <div class="m-4 text-xs text-center flex flex-col">
                    <span>${launchesData[launch]["name"]}</span>
                    <a class="underline text-blue-600" id="launchTitle${launch}" href="javascript:displayDetails('${launch}')">View Details</a>
                </div>
                <div id="launchDetail${launch}" class="m-4">
                    <span class="font-bold">${launchesData[launch]["name"]} <footer></footer></span>
                    <span class="block font-semibold text-sm">Rocket Name: ${launchesData[launch]["rocket"]["name"] || "Undefined"}</span>
                    <span class="block font-semibold  text-sm">Start Date: ${launchesData[launch]["status"]["window_start"] || "Undefined"}</span>
                    <span class="block font-semibold  text-sm">End Date: ${launchesData[launch]["status"]["window_end"] || "Undefined"}</span>
                    <span class="block font-semibold  text-sm">Agency Name: ${launchesData[launch]["provider"]["name"] || "Undefined"}</span>
                    <span class="block font-semibold  text-sm">Launchpad Name: ${launchesData[launch]["pad"]["name"] || "Undefined"}</span>
                    <span class="block font-semibold  text-sm">Location: ${launchesData[launch]["pad"]["location"]["name"] || "Undefined"}</span>
                    <span class="block font-semibold  text-sm">Mission Title: Unknown</span>

                </div>    
            `
        }
        container.append(itemCard);
    }
}

function displayDetails(id) {
    if (document.getElementById('launchDetail' + id).classList.contains("hidden")) {
        document.getElementById('launchDetail' + id).classList.remove("hidden")
        document.getElementById('launchDetail' + id).classList.add("visible");
        document.getElementById('launchTitle' + id).innerHTML="Hide Details"
    } else {
        document.getElementById('launchDetail' + id).classList.remove("visible");
        document.getElementById('launchDetail' + id).classList.add("hidden");
        document.getElementById('launchTitle' + id).innerHTML="View Details"
    }
}


function getQuery() {
    var url_string = window.location.href;
    var url = new URL(url_string);
    var usernameParam = url.searchParams.get("q");
    document.title = usernameParam;
    return usernameParam;
}

    //console.log(launchesData[launch]["name"].toLowerCase(), getQuery().toLowerCase())


