#include "pch.hpp"
#include "launch_entry.hpp"

std::optional<LaunchEntry> LaunchEntry::CreateLaunchEntry(nlohmann::json json){
    return std::nullopt; //an empty optional is returned. 
}
nlohmann::json LaunchEntry::ToJSON(){
    return nlohmann::json::object();
}

/*
Constructors
The first of each pair are the default constructors (used for making new of the object)
the second are the copy constructors (used for duplicating the object)
the third are the assignment operators (ie, a = b)
and the fourth are the move constructors (used for moving the object from one place in memory to another)
*/

LaunchStatus::LaunchStatus()
: code(LaunchCode::CONFIRMED), description(""), last_updated(std::chrono::system_clock::now()),
net(std::chrono::system_clock::now()), window_start(std::chrono::system_clock::now()), 
window_end(std::chrono::system_clock::now()), probability(0), holdreason("")
{}
LaunchStatus::LaunchStatus(const LaunchStatus& other)
: code(other.code), description(other.description), last_updated(other.last_updated),
net(other.net), window_start(other.window_start), window_end(other.window_end),
probability(other.probability), holdreason(other.holdreason)
{}
LaunchStatus& LaunchStatus::operator=(const LaunchStatus& other){
    code = other.code;
    description = other.description;
    last_updated = other.last_updated;
    net = other.net;
    window_start = other.window_start;
    window_end = other.window_end;
    probability = other.probability;
    holdreason = other.holdreason;
    return *this;
}
LaunchStatus::LaunchStatus(LaunchStatus&& other)
: code(std::move(other.code)), description(std::move(other.description)), 
last_updated(std::move(other.last_updated)), net(std::move(other.net)), 
window_start(std::move(other.window_start)), window_end(std::move(other.window_end)),
probability(other.probability), holdreason(std::move(other.holdreason))
{}

LaunchProvider::LaunchProvider()
: name(""), type(""), url("")
{}
LaunchProvider::LaunchProvider(const LaunchProvider& other)
: name(other.name), type(other.type), url(other.url)
{}
LaunchProvider& LaunchProvider::operator=(const LaunchProvider& other){
    name = other.name;
    type = other.type;
    url = other.url;
    return *this;
}
LaunchProvider::LaunchProvider(LaunchProvider&& other)
: name(std::move(other.name)), type(std::move(other.type)), url(std::move(other.url))
{}

LaunchVessel::LaunchVessel()
:name(""), family(""), variant(""), url("")
{}
LaunchVessel::LaunchVessel(const LaunchVessel& other)
: name(other.name), family(other.family), variant(other.variant), url(other.url)
{}
LaunchVessel& LaunchVessel::operator=(const LaunchVessel& other){
    name = other.name;
    family = other.family;
    variant = other.variant;
    url = other.url;
    return *this;
}
LaunchVessel::LaunchVessel(LaunchVessel&& other)
:name(std::move(other.name)), family(std::move(other.family)), 
variant(std::move(other.variant)), url(std::move(other.url))
{}

LaunchMission::LaunchMission()
:name(""), description(""), type(""), orbit_name(""), orbit_abbrev("")
{}
LaunchMission::LaunchMission(const LaunchMission& other)
: name(other.name), description(other.description), type(other.type),
orbit_name(other.orbit_name), orbit_abbrev(other.orbit_abbrev)
{}
LaunchMission& LaunchMission::operator=(const LaunchMission& other){
    name = other.name;
    description = other.description;
    type = other.type;
    orbit_name = other.orbit_name;
    orbit_abbrev = other.orbit_abbrev;
    return *this;
}
LaunchMission::LaunchMission(LaunchMission&& other)
: name(std::move(other.name)), description(std::move(other.description)), type(std::move(other.type)),
orbit_name(std::move(other.orbit_name)), orbit_abbrev(std::move(other.orbit_abbrev))
{}

LaunchPad::LaunchPad()
: name(""), url(""), map_url(""), latitude(0.0f), longitude(0.0f),
location_url(""), location_name(""), location_country("")
{}
LaunchPad::LaunchPad(const LaunchPad& other)
: name(other.name), url(other.url), map_url(other.map_url), 
latitude(other.latitude), longitude(other.longitude),
location_url(other.location_url), location_name(other.location_name), 
location_country(other.location_country)
{}
LaunchPad& LaunchPad::operator=(const LaunchPad& other){
    name = other.name;
    url = other.url;
    map_url = other.map_url;
    latitude = other.latitude;
    longitude = other.longitude;
    location_url = other.location_url;
    location_name = other.location_name;
    location_country = other.location_country;
    return *this;
}
LaunchPad::LaunchPad(LaunchPad&& other)
: name(std::move(other.name)), url(std::move(other.url)), map_url(std::move(other.map_url)), 
latitude(std::move(other.latitude)), longitude(std::move(other.longitude)),
location_url(std::move(other.location_url)), location_name(std::move(other.location_name)), 
location_country(std::move(other.location_country))
{}

LaunchEntry::LaunchEntry()
: uid(""), name(""), url(""),
status(), provider(), rocket(), mission(), pad() //these call the default constructors of those custom types
{}
LaunchEntry::LaunchEntry(const LaunchEntry& other)
: uid(other.uid), name(other.name), url(other.url),
status(other.status), provider(other.provider), rocket(other.rocket),
mission(other.mission), pad(other.pad)
{}
LaunchEntry& LaunchEntry::operator=(const LaunchEntry& other){
    uid = other.uid;
    name = other.name;
    url = other.url;
    status = other.status;
    provider = other.provider;
    rocket = other.rocket;
    mission = other.mission;
    pad = other.pad;
    return *this;
}
LaunchEntry::LaunchEntry(LaunchEntry&& other)
: uid(std::move(other.uid)), name(std::move(other.name)), url(std::move(other.url)),
status(std::move(other.status)), provider(std::move(other.provider)), 
rocket(std::move(other.rocket)), mission(std::move(other.mission)), pad(std::move(other.pad))
{}
