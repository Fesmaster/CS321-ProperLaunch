#include "pch.hpp"
#include "launch_entry.hpp"
#include "tests.hpp"


std::string LaunchCodeToText(LaunchCode code){
    COVERAGE_BRANCH
    switch(code){
        case LaunchCode::CONFIRMED : COVERAGE_BRANCH return "GO";
        case LaunchCode::UNCONFIRMED :COVERAGE_BRANCH return "TBC";
        case LaunchCode::UNDETERMINED :COVERAGE_BRANCH return "TBD";
        case LaunchCode::HELD :COVERAGE_BRANCH return "HELD";
        default :COVERAGE_BRANCH return "TBD"; //default error case
    }
}


LaunchCode InterpretSpaceDevLaunchAbbrev(std::string code){
    COVERAGE_BRANCH
    if (code == "TBC") {COVERAGE_BRANCH return LaunchCode::UNCONFIRMED;}
    else if (code == "TBD") {COVERAGE_BRANCH return LaunchCode::UNDETERMINED;}
    else if (code == "Go") {COVERAGE_BRANCH return LaunchCode::CONFIRMED;}
    //what exactly the code is for "held" is unknown right now
    else {COVERAGE_BRANCH return LaunchCode::UNDETERMINED;}
}


std::string TimeToText(const DateTime& time){
    COVERAGE_BRANCH
    std::stringstream ss;
    std::time_t t_c = std::chrono::system_clock::to_time_t(time);
    ss << std::put_time(std::gmtime(&t_c), "%FT%TZ" );
    return ss.str();
}

DateTime TextToTime(const std::string& text){
    COVERAGE_BRANCH
    std::stringstream ss(text);
    std::tm tm = {};
    //yes, this is different from the format string in TimeToText, but std::get_time does not
    //support the same format options (IE, %F)
    ss >> std::get_time(&tm, "%Y-%m-%dT%TZ"); 
    return std::chrono::system_clock::from_time_t(timegm(&tm));
}

//NEEDS TESTING
std::optional<LaunchEntry> LaunchEntry::CreateLaunchEntry(nlohmann::json json){
    COVERAGE_BRANCH
    if (json.is_object()){
        COVERAGE_BRANCH
        LaunchEntry e{};
        if (json["id"].is_string()){
            COVERAGE_BRANCH
            e.uid = json["id"].get<std::string>();
        }
        else{
            COVERAGE_BRANCH
            LOG_S(ERROR) << "LaunchEntry ID unable to import. Key is not in source. Aborting parse.";
            return std::nullopt;
        }

        if (json["url"].is_string()){
            COVERAGE_BRANCH
            e.url = json["url"].get<std::string>();
        }
        else{
            COVERAGE_BRANCH
            LOG_S(WARNING) << "LaunchEntry url unable to import. Key is not in source.";
        }

        if (json["image"].is_string()){
            COVERAGE_BRANCH
            e.image_url = json["image"].get<std::string>();
        }
        else{
            COVERAGE_BRANCH
            LOG_S(WARNING) << "LaunchEntry image_url unable to import. Key is not in source.";
        }

        if (json["name"].is_string()){
            COVERAGE_BRANCH
            e.name = json["name"].get<std::string>();
        }
        else{
            COVERAGE_BRANCH
            LOG_S(ERROR) << "LaunchEntry name unable to import. Key is not in source. Aborting parse.";
            return std::nullopt;
        }

        if (json["status"].is_object() && json["status"]["abbrev"].is_string()){
            COVERAGE_BRANCH
            e.status.code = InterpretSpaceDevLaunchAbbrev(json["status"]["abbrev"].get<std::string>());
        }
        else{
            COVERAGE_BRANCH
            LOG_S(ERROR) << "LaunchEntry status abbrev unable to import. Key is not in source. Aborting parse.";
            return std::nullopt;
        }

        if (json["status"].is_object() && json["status"]["description"].is_string()){
            COVERAGE_BRANCH
            e.status.description = json["status"]["description"].get<std::string>();
        }
        else{
            COVERAGE_BRANCH
            LOG_S(WARNING) << "LaunchEntry status description unable to import. Key is not in source. Aborting parse.";
        }

        if (json["last_updated"].is_string()){
            COVERAGE_BRANCH
            e.status.last_updated = TextToTime(json["last_updated"].get<std::string>());
        }
        else{
            COVERAGE_BRANCH
            LOG_S(ERROR) << "LaunchEntry last updated time unable to import. Key is not in source. Aborting parse.";
            return std::nullopt;
        }

        if (json["window_end"].is_string()){
            COVERAGE_BRANCH
            e.status.window_end = TextToTime(json["window_end"].get<std::string>());
        }
        else{
            COVERAGE_BRANCH
            LOG_S(ERROR) << "LaunchEntry window end time unable to import. Key is not in source. Aborting parse.";
            return std::nullopt;
        }

        if (json["window_start"].is_string()){
            COVERAGE_BRANCH
            e.status.window_start = TextToTime(json["window_start"].get<std::string>());
        }
        else{
            COVERAGE_BRANCH
            return std::nullopt;
            LOG_S(ERROR) << "LaunchEntry window start time unable to import. Key is not in source. Aborting parse.";
        }

        if (json["net"].is_string()){
            COVERAGE_BRANCH
            e.status.net = TextToTime(json["net"].get<std::string>());
        }
        else{
            COVERAGE_BRANCH
            LOG_S(WARNING) << "LaunchEntry net time unable to import. Key is not in source. Using window_start.";
            e.status.net = e.status.window_start;
        }

        if (json["probability"].is_number()){
            COVERAGE_BRANCH
            e.status.probability = json["probability"].get<int>();
        }
        else{
            COVERAGE_BRANCH
            LOG_S(WARNING) << "LaunchEntry probability unable to import. Key is not in source.";
        }

        if (json["holdreason"].is_string()){
            COVERAGE_BRANCH
            e.status.holdreason = json["holdreason"].get<std::string>();
        }
        else{
            COVERAGE_BRANCH
            LOG_S(WARNING) << "LaunchEntry holdreason unable to import. Key is not in source.";
        }

        auto provider = json["launch_service_provider"];
        if (provider.is_object()){
            COVERAGE_BRANCH

            if (provider["name"].is_string()){
                COVERAGE_BRANCH
                e.provider.name = provider["name"].get<std::string>();
            }else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "Unknown launch provider name was encoutnered. The key was not a string in source.";
            }

            if (provider["type"].is_string()){
                COVERAGE_BRANCH
                e.provider.type = provider["type"].get<std::string>();
            }else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "Unknown launch provider type was encoutnered. The key was not a string in source.";
            }

            if (provider["url"].is_string()){
                COVERAGE_BRANCH
                e.provider.url = provider["url"].get<std::string>();
            }else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "Unknown launch provider url was encoutnered. The key was not a string in source.";
            }
        }
        else{
            COVERAGE_BRANCH
            LOG_S(WARNING) << "An unknown launch provider was encountered - the whole key \"launch_service_provider\" was not an object in source";
        }

        auto rocket = json["rocket"];
        if (rocket.is_object()){
            COVERAGE_BRANCH
            auto rocket_config = rocket["configuration"];
            if (rocket_config.is_object()){
                COVERAGE_BRANCH
                if (rocket_config["name"].is_string()){
                    COVERAGE_BRANCH
                    e.rocket.name = rocket_config["name"].get<std::string>();
                }
                else{
                    COVERAGE_BRANCH
                    LOG_S(WARNING) << "An unknown rocket name was encountered. The key was not a string in source.";
                }

                if (rocket_config["family"].is_string()){
                    COVERAGE_BRANCH
                    e.rocket.family = rocket_config["family"].get<std::string>();
                }
                else{
                    COVERAGE_BRANCH
                    LOG_S(WARNING) << "An unknown rocket family was encountered. The key was not a string in source.";
                }

                if (rocket_config["variant"].is_string()){
                    COVERAGE_BRANCH
                    e.rocket.variant = rocket_config["variant"].get<std::string>();
                }
                else{
                    COVERAGE_BRANCH
                    LOG_S(WARNING) << "An unknown rocket variant was encountered. The key was not a string in source.";
                }

                if (rocket_config["url"].is_string()){
                    COVERAGE_BRANCH
                    e.rocket.url = rocket_config["url"].get<std::string>();
                }
                else{
                    COVERAGE_BRANCH
                    LOG_S(WARNING) << "An unknown rocket url was encountered. The key was not a string in source.";
                }
            }
            else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "An unknown rocket was encoutntered - the subkey \"configuration\" was not an object in source";
            }
        }else{
            COVERAGE_BRANCH
            LOG_S(WARNING) << "An unknown rocket was encountered - the whole key \"rocket\" was not an object in source";
        }

        auto mission = json["mission"];
        if (mission.is_object()){
            COVERAGE_BRANCH
            if (mission["name"].is_string()){
                COVERAGE_BRANCH
                e.mission.name = mission["name"].get<std::string>();
            }
            else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "An unknown mission name was encoutered. The key was not a string in source.";
            }

            if (mission["description"].is_string()){
                COVERAGE_BRANCH
                e.mission.description = mission["description"].get<std::string>();
            }
            else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "An unknown mission description was encoutered. The key was not a string in source.";
            }

            if (mission["type"].is_string()){
                COVERAGE_BRANCH
                e.mission.type = mission["type"].get<std::string>();
            }
            else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "An unknown mission type was encoutered. The key was not a string in source.";
            }
            auto orbit = mission["orbit"];
            if (orbit.is_object()){
                COVERAGE_BRANCH
                if (orbit["name"].is_string()){
                    COVERAGE_BRANCH
                    e.mission.orbit_name = orbit["name"].get<std::string>();
                }
                else{
                    COVERAGE_BRANCH
                    LOG_S(WARNING) << "Unknown mission orbit name was encounteres. The key was not a string in source";
                }

                if (orbit["abbrev"].is_string()){
                    COVERAGE_BRANCH
                    e.mission.orbit_abbrev = orbit["abbrev"].get<std::string>();
                }
                else{
                    COVERAGE_BRANCH
                    LOG_S(WARNING) << "Unknown mission orbit abbrev was encounteres. The key was not a string in source";
                }
            }
            else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "An unknown mission orbit was encoutered. The key was not an object in source.";
            }

        }
        else{
            COVERAGE_BRANCH
            LOG_S(WARNING) << "An unknown mission was encountered - the whole key \"mission\" was not an object in source";
        }

        auto pad = json["pad"];
        if (pad.is_object()){
            COVERAGE_BRANCH
            if (pad["name"].is_string()){
                COVERAGE_BRANCH
                e.pad.name = pad["name"].get<std::string>();
            }
            else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "An unknown pad name was encountered. The key was not a string in source";
            }

            if (pad["url"].is_string()){
                COVERAGE_BRANCH
                e.pad.url = pad["url"].get<std::string>();
            }
            else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "An unknown pad url was encountered. The key was not a string in source";
            }

            if (pad["map_url"].is_string()){
                COVERAGE_BRANCH
                e.pad.map_url = pad["map_url"].get<std::string>();
            }
            else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "An unknown pad map_url was encountered. The key was not a string in source";
            }

            if (pad["latitude"].is_string()){
                COVERAGE_BRANCH
                std::stringstream ss(pad["latitude"].get<std::string>());
                ss >> e.pad.latitude;
            }
            else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "An unknown pad latitude was encountered. The key was not a string in source";
            }

            if (pad["longitude"].is_string()){
                COVERAGE_BRANCH
                std::stringstream ss(pad["longitude"].get<std::string>());
                ss >> e.pad.longitude;
            }
            else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "An unknown pad longitude was encountered. The key was not a string in source";
            }
            auto location = pad["location"];
            if (location.is_object()){
                COVERAGE_BRANCH
                if (location["name"].is_string()){
                    COVERAGE_BRANCH
                    e.pad.location_name = location["name"].get<std::string>();
                }
                else{
                    COVERAGE_BRANCH
                    LOG_S(WARNING) << "An unknown pad location name was encountered. The key was not a string in source";
                }

                if (location["country_code"].is_string()){
                    COVERAGE_BRANCH
                    e.pad.location_country = location["country_code"].get<std::string>();
                }
                else{
                    COVERAGE_BRANCH
                    LOG_S(WARNING) << "An unknown pad location country_code was encountered. The key was not a string in source";
                }

                if (location["url"].is_string()){
                    COVERAGE_BRANCH
                    e.pad.location_url = location["url"].get<std::string>();
                }
                else{
                    COVERAGE_BRANCH
                    LOG_S(WARNING) << "An unknown pad location url was encountered. The key was not a string in source";
                }
            }
            else{
                COVERAGE_BRANCH
                LOG_S(WARNING) << "An unknown pad location was encountered. The key was not an object in source";
            }
        }
        else{
            COVERAGE_BRANCH
            LOG_S(WARNING) << "An unknown pad was encountered - the whole key \"pad\" was not an object in source";
        }

        return std::optional<LaunchEntry>(std::move(e));
    }
    COVERAGE_BRANCH_ELSE
    LOG_S(ERROR) << "Unable to parse a json object that is not a full object for a LaunchEntry. Arrays and subtypes are not allowed. Aborting parse.";
    return std::nullopt; //an empty optional is returned. 
}

//NEEDS TESTING
nlohmann::json LaunchEntry::ToJSON() const{
    COVERAGE_BRANCH
    return nlohmann::json({
        {"name", name},
        {"uid", uid},
        {"url", url},
        {"image_url", image_url},
        {"status", {
            {"code", LaunchCodeToText(status.code)},
            {"description", status.description},
            {"last_updated", TimeToText(status.last_updated)},
            {"net", TimeToText(status.net)},
            {"window_start", TimeToText(status.window_start)},
            {"window_end", TimeToText(status.window_end)},
            {"probability", status.probability},
            {"holdreason", status.holdreason}
        }},
        {"provider", {
            {"name", provider.name},
            {"type", provider.type},
            {"url", provider.url}
        }},
        {"rocket", {
            {"name", rocket.name},
            {"family", rocket.family},
            {"variant", rocket.variant},
            {"url", rocket.url}
        }},
        {"mission", {
            {"name", mission.name},
            {"description", mission.description},
            {"type", mission.type},
            {"orbit", {
                {"name", mission.orbit_name},
                {"abbrev", mission.orbit_abbrev}
            }}
        }},
        {"pad", {
            {"name", pad.name},
            {"url", pad.url},
            {"map_url", pad.map_url},
            {"latitude", pad.latitude},
            {"longitude", pad.longitude},
            {"location", {
                {"name", pad.location_name},
                {"country", pad.location_country},
                {"url", pad.location_url}
            }},
        }}
    });
}


std::string str_tolower(const std::string& str){
    COVERAGE_BRANCH
    std::string cpy(str);
    std::transform(str.begin(), str.end(), cpy.begin(), [](unsigned char c){return std::tolower(c);});
    return cpy;
}

bool LaunchEntry::DoesMatchString(const std::string& other) const{
    COVERAGE_BRANCH
    std::string lowerSearch = str_tolower(other);
    if (
        (str_tolower(name).find(lowerSearch) != std::string::npos) ||
        (str_tolower(LaunchCodeToText(status.code)).find(lowerSearch) != std::string::npos) ||
        (str_tolower(status.description).find(lowerSearch) != std::string::npos) || 
        (str_tolower(provider.name).find(lowerSearch) != std::string::npos) || 
        (str_tolower(provider.type).find(lowerSearch) != std::string::npos) || 
        (str_tolower(rocket.name).find(lowerSearch) != std::string::npos) || 
        (str_tolower(rocket.family).find(lowerSearch) != std::string::npos) || 
        (str_tolower(rocket.variant).find(lowerSearch) != std::string::npos) || 
        (str_tolower(mission.name).find(lowerSearch) != std::string::npos) || 
        (str_tolower(mission.description).find(lowerSearch) != std::string::npos) || 
        (str_tolower(mission.type).find(lowerSearch) != std::string::npos) || 
        (str_tolower(mission.orbit_name).find(lowerSearch) != std::string::npos) || 
        (str_tolower(mission.orbit_abbrev).find(lowerSearch) != std::string::npos) || 
        (str_tolower(pad.name).find(lowerSearch) != std::string::npos) || 
        (str_tolower(pad.location_name).find(lowerSearch) != std::string::npos) || 
        (str_tolower(pad.location_country).find(lowerSearch) != std::string::npos)
    ){
        COVERAGE_BRANCH
        return true;
    }
    else{
        COVERAGE_BRANCH
        return DoesFieldMatchString(SortKey::DATE, other);
    }
}

bool LaunchEntry::DoesFieldMatchString(SortKey field, const std::string& search) const{
    COVERAGE_BRANCH
    std::string lowerSearch = str_tolower(search);
    switch(field){
        case SortKey::NAME : {
            COVERAGE_BRANCH
            if (str_tolower(name).find(lowerSearch) != std::string::npos){
                COVERAGE_BRANCH
                return true;
            }
            else{
                COVERAGE_BRANCH
                return false;
            }
        }
        case SortKey::ROCKET : {
            COVERAGE_BRANCH
            if ((str_tolower(rocket.name).find(lowerSearch) != std::string::npos) || 
                (str_tolower(rocket.family).find(lowerSearch) != std::string::npos) || 
                (str_tolower(rocket.variant).find(lowerSearch) != std::string::npos)
            ){
                COVERAGE_BRANCH
                return true;
            }
            else{
                COVERAGE_BRANCH
                return false;
            }
        }
        case SortKey::PROVIDER : {
            COVERAGE_BRANCH
            if ((str_tolower(provider.name).find(lowerSearch) != std::string::npos) || 
                (str_tolower(provider.type).find(lowerSearch) != std::string::npos)
            ){
                COVERAGE_BRANCH
                return true;
            }
            else{
                COVERAGE_BRANCH
                return false;
            }
        }
        case SortKey::MISSION : {
            COVERAGE_BRANCH
            if ((str_tolower(mission.name).find(lowerSearch) != std::string::npos) || 
                (str_tolower(mission.description).find(lowerSearch) != std::string::npos) || 
                (str_tolower(mission.type).find(lowerSearch) != std::string::npos) || 
                (str_tolower(mission.orbit_name).find(lowerSearch) != std::string::npos) || 
                (str_tolower(mission.orbit_abbrev).find(lowerSearch) != std::string::npos)
            ){
                COVERAGE_BRANCH
                return true;
            }
            else{
                COVERAGE_BRANCH
                return false;
            }
        }
        case SortKey::PAD : {
            COVERAGE_BRANCH
            if (str_tolower(pad.name).find(lowerSearch) != std::string::npos){
                COVERAGE_BRANCH
                return true;
            }
            else{
                COVERAGE_BRANCH
                return false;
            }
        }
        case SortKey::LOCATION : {
            COVERAGE_BRANCH
            if ((str_tolower(pad.location_name).find(lowerSearch) != std::string::npos) || 
                (str_tolower(pad.location_country).find(lowerSearch) != std::string::npos)
            ){
                COVERAGE_BRANCH
                return true;
            }
            else{
                COVERAGE_BRANCH
                return false;
            }
        }
        case SortKey::DATE : {
            COVERAGE_BRANCH
            //the special one
            DateTime d = TextToTime(search);
            if (d <= status.window_end && d >= status.window_start){
                COVERAGE_BRANCH
                return true;
            }
            else{
                COVERAGE_BRANCH
                return false;
            }
        }
        default: {
            COVERAGE_BRANCH
            return DoesMatchString(search);
        }
    }
}


/*
Constructors
The first of each pair are the default constructors (used for making new of the object)
the second are the copy constructors (used for duplicating the object)
the third are the assignment operators (ie, a = b)
and the fourth are the move constructors (used for moving the object from one place in memory to another)
*/

LaunchStatus::LaunchStatus()
: code(LaunchCode::CONFIRMED), description("Status discripion unavailable"), last_updated(std::chrono::system_clock::now()),
net(std::chrono::system_clock::now()), window_start(std::chrono::system_clock::now()), 
window_end(std::chrono::system_clock::now()), probability(-1), holdreason("")
{COVERAGE_BRANCH}
LaunchStatus::LaunchStatus(const LaunchStatus& other)
: code(other.code), description(other.description), last_updated(other.last_updated),
net(other.net), window_start(other.window_start), window_end(other.window_end),
probability(other.probability), holdreason(other.holdreason)
{COVERAGE_BRANCH}
LaunchStatus& LaunchStatus::operator=(const LaunchStatus& other){
    COVERAGE_BRANCH
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
{COVERAGE_BRANCH}

LaunchProvider::LaunchProvider()
: name("Unknown"), type("Unknown"), url("")
{COVERAGE_BRANCH}
LaunchProvider::LaunchProvider(const LaunchProvider& other)
: name(other.name), type(other.type), url(other.url)
{COVERAGE_BRANCH}
LaunchProvider& LaunchProvider::operator=(const LaunchProvider& other){
    COVERAGE_BRANCH
    name = other.name;
    type = other.type;
    url = other.url;
    return *this;
}
LaunchProvider::LaunchProvider(LaunchProvider&& other)
: name(std::move(other.name)), type(std::move(other.type)), url(std::move(other.url))
{COVERAGE_BRANCH}

LaunchVessel::LaunchVessel()
:name("Unknown"), family("Unknown"), variant("Unknown"), url("")
{COVERAGE_BRANCH}
LaunchVessel::LaunchVessel(const LaunchVessel& other)
: name(other.name), family(other.family), variant(other.variant), url(other.url)
{COVERAGE_BRANCH}
LaunchVessel& LaunchVessel::operator=(const LaunchVessel& other){
    COVERAGE_BRANCH
    name = other.name;
    family = other.family;
    variant = other.variant;
    url = other.url;
    return *this;
}
LaunchVessel::LaunchVessel(LaunchVessel&& other)
:name(std::move(other.name)), family(std::move(other.family)), 
variant(std::move(other.variant)), url(std::move(other.url))
{COVERAGE_BRANCH}

LaunchMission::LaunchMission()
:name("Unknown"), description("Unknown"), type("Unknown"), 
orbit_name("Unknown"), orbit_abbrev("Unknown")
{COVERAGE_BRANCH}
LaunchMission::LaunchMission(const LaunchMission& other)
: name(other.name), description(other.description), type(other.type),
orbit_name(other.orbit_name), orbit_abbrev(other.orbit_abbrev)
{COVERAGE_BRANCH}
LaunchMission& LaunchMission::operator=(const LaunchMission& other){
    COVERAGE_BRANCH
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
{COVERAGE_BRANCH}

LaunchPad::LaunchPad()
: name("Unknown"), url(""), map_url(""), latitude(0.0f), longitude(0.0f),
location_url(""), location_name("Unknown"), location_country("Unknown")
{COVERAGE_BRANCH}
LaunchPad::LaunchPad(const LaunchPad& other)
: name(other.name), url(other.url), map_url(other.map_url), 
latitude(other.latitude), longitude(other.longitude),
location_url(other.location_url), location_name(other.location_name), 
location_country(other.location_country)
{COVERAGE_BRANCH}
LaunchPad& LaunchPad::operator=(const LaunchPad& other){
    COVERAGE_BRANCH
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
{COVERAGE_BRANCH}

LaunchEntry::LaunchEntry()
: uid(""), name(""), url(""), image_url(""),
status(), provider(), rocket(), mission(), pad() //these call the default constructors of those custom types
{COVERAGE_BRANCH}
LaunchEntry::LaunchEntry(const LaunchEntry& other)
: uid(other.uid), name(other.name), url(other.url), image_url(other.image_url),
status(other.status), provider(other.provider), rocket(other.rocket),
mission(other.mission), pad(other.pad)
{COVERAGE_BRANCH}
LaunchEntry& LaunchEntry::operator=(const LaunchEntry& other){
    COVERAGE_BRANCH
    uid = other.uid;
    name = other.name;
    url = other.url;
    image_url = other.image_url;
    status = other.status;
    provider = other.provider;
    rocket = other.rocket;
    mission = other.mission;
    pad = other.pad;
    return *this;
}
LaunchEntry::LaunchEntry(LaunchEntry&& other)
: uid(std::move(other.uid)), name(std::move(other.name)), url(std::move(other.url)),
image_url(std::move(other.image_url)), status(std::move(other.status)), provider(std::move(other.provider)), 
rocket(std::move(other.rocket)), mission(std::move(other.mission)), pad(std::move(other.pad))
{COVERAGE_BRANCH}


#ifdef TESTS_ENABLED
//tests

void launch_entry_tests(){
    LOG_S(INFO) << "Starting LaunchEntry tests";
    LOG_S(INFO) << "Expect a bunch of yellow and red lines. As long as it does not crash, it is passing.";
    

    CHECK_EQ_F(LaunchCodeToText(LaunchCode::CONFIRMED), "GO", "LaunchCodeToText failure.");
    CHECK_EQ_F(LaunchCodeToText(LaunchCode::UNCONFIRMED), "TBC", "LaunchCodeToText failure.");
    CHECK_EQ_F(LaunchCodeToText(LaunchCode::UNDETERMINED), "TBD", "LaunchCodeToText failure.");
    CHECK_EQ_F(LaunchCodeToText(LaunchCode::HELD), "HELD", "LaunchCodeToText failure.");
    CHECK_EQ_F(LaunchCodeToText(((LaunchCode)100)), "TBD", "LaunchCodeToText failure."); //intentional stupid code
    
    CHECK_EQ_F(InterpretSpaceDevLaunchAbbrev("Go"), LaunchCode::CONFIRMED, "InterpretSpaceDevLaunchAbbrev failed on \"GO\"");
    CHECK_EQ_F(InterpretSpaceDevLaunchAbbrev("TBC"), LaunchCode::UNCONFIRMED, "InterpretSpaceDevLaunchAbbrev failed on \"TBC\"");
    CHECK_EQ_F(InterpretSpaceDevLaunchAbbrev("TBD"), LaunchCode::UNDETERMINED, "InterpretSpaceDevLaunchAbbrev failed on \"TBD\"");
    CHECK_EQ_F(InterpretSpaceDevLaunchAbbrev("anything else"), LaunchCode::UNDETERMINED, "InterpretSpaceDevLaunchAbbrev failed on default case");
    

    std::array<std::string, 6> times = {"2022-05-25T18:27:00Z", "2022-05-18T13:08:58Z", "2022-05-31T13:30:00Z",
    "2022-05-31T13:30:00Z", "2022-05-17T14:08:21Z","2022-12-31T00:00:00Z"};
    for (auto t: times){
        //these two functions cannot really exist without the other, as the internal DateTime object has no real
        //interface.
        CHECK_EQ_F(t, TimeToText(TextToTime(t)), "Date-Time conversion system failed on time: %s", t.c_str());
    }

    //test the actual data generation
    auto launchSource1 = R"QERXY(
        {
			"id": "949421ac-3802-499b-b383-d8274de7e147",
			"url": "https://lldev.thespacedevs.com/2.2.0/launch/949421ac-3802-499b-b383-d8274de7e147/",
			"slug": "falcon-9-block-5-transporter-5-dedicated-sso-rides",
			"name": "Falcon 9 Block 5 | Transporter 5 (Dedicated SSO Rideshare)",
			"status": {
				"id": 8,
				"name": "To Be Confirmed",
				"abbrev": "TBC",
				"description": "Awaiting official confirmation - current date is known with some certainty."
			},
			"last_updated": "2022-05-24T05:01:56Z",
			"net": "2022-05-25T18:27:00Z",
			"window_end": "2022-05-25T18:27:00Z",
			"window_start": "2022-05-25T18:27:00Z",
			"probability": 80,
			"holdreason": "",
			"failreason": "",
			"hashtag": null,
			"launch_service_provider": {
				"id": 121,
				"url": "https://lldev.thespacedevs.com/2.2.0/agencies/121/",
				"name": "SpaceX",
				"type": "Commercial"
			},
			"rocket": {
				"id": 7501,
				"configuration": {
					"id": 164,
					"url": "https://lldev.thespacedevs.com/2.2.0/config/launcher/164/",
					"name": "Falcon 9",
					"family": "Falcon",
					"full_name": "Falcon 9 Block 5",
					"variant": "Block 5"
				}
			},
			"mission": {
				"id": 5900,
				"name": "Transporter 5 (Dedicated SSO Rideshare)",
				"description": "Transporter 5 mission is a dedicated rideshare flight to a sun-synchronous orbit with dozens of small microsatellites and nanosatellites for commercial and government customers.",
				"launch_designator": null,
				"type": "Dedicated Rideshare",
				"orbit": {
					"id": 17,
					"name": "Sun-Synchronous Orbit",
					"abbrev": "SSO"
				}
			},
			"pad": {
				"id": 80,
				"url": "https://lldev.thespacedevs.com/2.2.0/pad/80/",
				"agency_id": 121,
				"name": "Space Launch Complex 40",
				"info_url": null,
				"wiki_url": "https://en.wikipedia.org/wiki/Cape_Canaveral_Air_Force_Station_Space_Launch_Complex_40",
				"map_url": "http://maps.google.com/maps?q=28.56194122,-80.57735736",
				"latitude": "28.56194122",
				"longitude": "-80.57735736",
				"location": {
					"id": 12,
					"url": "https://lldev.thespacedevs.com/2.2.0/location/12/",
					"name": "Cape Canaveral, FL, USA",
					"country_code": "USA",
					"map_image": "https://spacelaunchnow-prod-east.nyc3.digitaloceanspaces.com/media/launch_images/location_12_20200803142519.jpg",
					"total_launch_count": 843,
					"total_landing_count": 23
				},
				"map_image": "https://spacelaunchnow-prod-east.nyc3.digitaloceanspaces.com/media/launch_images/pad_80_20200803143323.jpg",
				"total_launch_count": 142
			},
			"webcast_live": false,
			"image": "https://spacelaunchnow-prod-east.nyc3.digitaloceanspaces.com/media/launcher_images/falcon_9_block__image_20210506060831.jpg",
			"infographic": null,
			"program": []
		}
    )QERXY"_json;
    
    std::optional<LaunchEntry> entry1 = LaunchEntry::CreateLaunchEntry(launchSource1);
    CHECK_F(entry1.has_value(), "LaunchEntry::Create should have returned a valid value.");
    CHECK_EQ_F(entry1->name, "Falcon 9 Block 5 | Transporter 5 (Dedicated SSO Rideshare)", "Name was improperly imported.");
    CHECK_EQ_F(entry1->uid, "949421ac-3802-499b-b383-d8274de7e147", "ID was improperly imported.");
    CHECK_EQ_F(entry1->url, "https://lldev.thespacedevs.com/2.2.0/launch/949421ac-3802-499b-b383-d8274de7e147/", "url was improperly imported.");
    
    CHECK_EQ_F(entry1->status.code, LaunchCode::UNCONFIRMED, "status code was improperly imported.");
    CHECK_EQ_F(entry1->status.description, "Awaiting official confirmation - current date is known with some certainty.", "status description was improperly imported.");
    CHECK_EQ_F(TimeToText(entry1->status.last_updated), "2022-05-24T05:01:56Z", "last _updated was improperly imported.");
    CHECK_EQ_F(TimeToText(entry1->status.net), "2022-05-25T18:27:00Z", "net was improperly imported.");
    CHECK_EQ_F(TimeToText(entry1->status.window_start), "2022-05-25T18:27:00Z", "window_start was improperly imported.");
    CHECK_EQ_F(TimeToText(entry1->status.window_end), "2022-05-25T18:27:00Z", "window_end was improperly imported.");
    CHECK_EQ_F(entry1->status.probability, 80, "probability was improperly imported.");
    CHECK_EQ_F(entry1->status.holdreason, "", "holdreason was improperly imported.");
    
    CHECK_EQ_F(entry1->provider.name, "SpaceX", "provider name was improperly imported.");
    CHECK_EQ_F(entry1->provider.type, "Commercial", "provider type was improperly imported.");
    CHECK_EQ_F(entry1->provider.url, "https://lldev.thespacedevs.com/2.2.0/agencies/121/", "provider url was improperly imported.");

    CHECK_EQ_F(entry1->rocket.name, "Falcon 9", "rocket name was improperly imported.");
    CHECK_EQ_F(entry1->rocket.family, "Falcon", "rocket family was improperly imported.");
    CHECK_EQ_F(entry1->rocket.variant, "Block 5", "rocket variant was improperly imported.");
    CHECK_EQ_F(entry1->rocket.url, "https://lldev.thespacedevs.com/2.2.0/config/launcher/164/", "rocket url was improperly imported.");
    
    CHECK_EQ_F(entry1->mission.name, "Transporter 5 (Dedicated SSO Rideshare)", "mission name was improperly imported.");
    CHECK_EQ_F(entry1->mission.description, "Transporter 5 mission is a dedicated rideshare flight to a sun-synchronous orbit with dozens of small microsatellites and nanosatellites for commercial and government customers.", "mission description was improperly imported.");
    CHECK_EQ_F(entry1->mission.type, "Dedicated Rideshare", "mission type was improperly imported.");
    CHECK_EQ_F(entry1->mission.orbit_name, "Sun-Synchronous Orbit", "mission orbit_name was improperly imported.");
    CHECK_EQ_F(entry1->mission.orbit_abbrev, "SSO", "mission orbit_abbrev was improperly imported.");
    
    CHECK_EQ_F(entry1->pad.name, "Space Launch Complex 40", "pad name was improperly imported.");
    CHECK_EQ_F(entry1->pad.url, "https://lldev.thespacedevs.com/2.2.0/pad/80/", "pad url was improperly imported.");
    CHECK_EQ_F(entry1->pad.map_url, "http://maps.google.com/maps?q=28.56194122,-80.57735736", "pad map_url was improperly imported.");
    CHECK_EQ_F(entry1->pad.latitude, 28.56194122f, "pad latitude was improperly imported.");
    CHECK_EQ_F(entry1->pad.longitude, -80.57735736f, "pad longitude was improperly imported.");
    CHECK_EQ_F(entry1->pad.location_name, "Cape Canaveral, FL, USA", "pad location_name was improperly imported.");
    CHECK_EQ_F(entry1->pad.location_country, "USA", "pad location_country was improperly imported.");
    CHECK_EQ_F(entry1->pad.location_url, "https://lldev.thespacedevs.com/2.2.0/location/12/", "pad location_url was improperly imported.");
    
    auto json1 = entry1->ToJSON();
    CHECK_EQ_F(json1["name"], "Falcon 9 Block 5 | Transporter 5 (Dedicated SSO Rideshare)", "Name was improperly exported.");
    CHECK_EQ_F(json1["uid"], "949421ac-3802-499b-b383-d8274de7e147", "ID was improperly exported.");
    CHECK_EQ_F(json1["url"], "https://lldev.thespacedevs.com/2.2.0/launch/949421ac-3802-499b-b383-d8274de7e147/", "url was improperly exported.");
    
    CHECK_EQ_F(json1["status"]["code"], "TBC", "status code was improperly exported.");
    CHECK_EQ_F(json1["status"]["description"], "Awaiting official confirmation - current date is known with some certainty.", "status description was improperly exported.");
    CHECK_EQ_F(json1["status"]["last_updated"], "2022-05-24T05:01:56Z", "last _updated was improperly exported.");
    CHECK_EQ_F(json1["status"]["net"], "2022-05-25T18:27:00Z", "net was improperly exported.");
    CHECK_EQ_F(json1["status"]["window_start"], "2022-05-25T18:27:00Z", "window_start was improperly exported.");
    CHECK_EQ_F(json1["status"]["window_end"], "2022-05-25T18:27:00Z", "window_end was improperly exported.");
    CHECK_EQ_F(json1["status"]["probability"], 80, "probability was improperly exported.");
    CHECK_EQ_F(json1["status"]["holdreason"], "", "holdreason was improperly exported.");
    
    CHECK_EQ_F(json1["provider"]["name"], "SpaceX", "provider name was improperly exported.");
    CHECK_EQ_F(json1["provider"]["type"], "Commercial", "provider type was improperly exported.");
    CHECK_EQ_F(json1["provider"]["url"], "https://lldev.thespacedevs.com/2.2.0/agencies/121/", "provider url was improperly exported.");

    CHECK_EQ_F(json1["rocket"]["name"], "Falcon 9", "rocket name was improperly exported.");
    CHECK_EQ_F(json1["rocket"]["family"], "Falcon", "rocket family was improperly exported.");
    CHECK_EQ_F(json1["rocket"]["variant"], "Block 5", "rocket variant was improperly exported.");
    CHECK_EQ_F(json1["rocket"]["url"], "https://lldev.thespacedevs.com/2.2.0/config/launcher/164/", "rocket url was improperly exported.");
    
    CHECK_EQ_F(json1["mission"]["name"], "Transporter 5 (Dedicated SSO Rideshare)", "mission name was improperly exported.");
    CHECK_EQ_F(json1["mission"]["description"], "Transporter 5 mission is a dedicated rideshare flight to a sun-synchronous orbit with dozens of small microsatellites and nanosatellites for commercial and government customers.", "mission description was improperly exported.");
    CHECK_EQ_F(json1["mission"]["type"], "Dedicated Rideshare", "mission type was improperly exported.");
    CHECK_EQ_F(json1["mission"]["orbit"]["name"], "Sun-Synchronous Orbit", "mission orbit_name was improperly exported.");
    CHECK_EQ_F(json1["mission"]["orbit"]["abbrev"], "SSO", "mission orbit_abbrev was improperly exported.");
    
    CHECK_EQ_F(json1["pad"]["name"], "Space Launch Complex 40", "pad name was improperly exported.");
    CHECK_EQ_F(json1["pad"]["url"], "https://lldev.thespacedevs.com/2.2.0/pad/80/", "pad url was improperly exported.");
    CHECK_EQ_F(json1["pad"]["map_url"], "http://maps.google.com/maps?q=28.56194122,-80.57735736", "pad map_url was improperly exported.");
    CHECK_EQ_F(json1["pad"]["latitude"], 28.56194122f, "pad latitude was improperly exported.");
    CHECK_EQ_F(json1["pad"]["longitude"], -80.57735736f, "pad longitude was improperly exported.");
    CHECK_EQ_F(json1["pad"]["location"]["name"], "Cape Canaveral, FL, USA", "pad location_name was improperly exported.");
    CHECK_EQ_F(json1["pad"]["location"]["country"], "USA", "pad location_country was improperly exported.");
    CHECK_EQ_F(json1["pad"]["location"]["url"], "https://lldev.thespacedevs.com/2.2.0/location/12/", "pad location_url was improperly exported.");



    auto launchSource2 = R"QERXY(
        {
			"id": "f058ecca-bda7-4797-ae47-b5c450b1bd78",
			"slug": "new-shepard-ns-21",
			"name": "New Shepard | NS-21",
			"status": {
				"id": 2,
				"name": "To Be Determined",
				"abbrev": "TBD"
			},
			"last_updated": "2022-05-18T13:08:58Z",
			"window_end": "2022-05-31T16:30:00Z",
			"window_start": "2022-05-31T13:30:00Z",
			"failreason": "",
			"hashtag": null,
			"launch_service_provider": {
				"id": 141
			},
			"rocket": {
				"id": 7573,
				"configuration": {
					"id": 137,
					"full_name": "New Shepard"
				}
			},
			"mission": {
				"id": 6016,
				"launch_designator": null,
				"orbit": {
					"id": 15
				}
			},
			"pad": {
				"id": 90,
				"agency_id": 141,
				"info_url": "http://www.blueorigin.com",
				"wiki_url": "https://en.wikipedia.org/wiki/Corn_Ranch",
				"location": {
					"id": 29,
					"map_image": "https://spacelaunchnow-prod-east.nyc3.digitaloceanspaces.com/media/launch_images/location_29_20200803142436.jpg",
					"total_launch_count": 21,
					"total_landing_count": 0
				},
				"map_image": "https://spacelaunchnow-prod-east.nyc3.digitaloceanspaces.com/media/launch_images/pad_90_20200803143233.jpg",
				"total_launch_count": 21
			},
			"webcast_live": false,
			"image": "https://spacelaunchnow-prod-east.nyc3.digitaloceanspaces.com/media/launcher_images/new2520shepard_image_20190207032624.jpeg",
			"infographic": null,
			"program": []
		}
    )QERXY"_json;
    
    std::optional<LaunchEntry> entry2 = LaunchEntry::CreateLaunchEntry(launchSource2);
    CHECK_F(entry2.has_value(), "Failed to get the value of entry 2, a slightly information-less attempt.");

    CHECK_EQ_F(entry2->url, "",                                 "url was improperly defaulted.");
    CHECK_EQ_F(entry2->status.description, "Status discripion unavailable", "status description was improperly defaulted.");
    CHECK_EQ_F(entry2->status.net, entry2->status.window_start, "net was improperly defaulted.");
    CHECK_EQ_F(entry2->status.probability, -1,                  "probability was improperly defaulted.");
    CHECK_EQ_F(entry2->status.holdreason, "",                   "holdreason was improperly defaulted.");
    CHECK_EQ_F(entry2->provider.name, "Unknown",                "provider name was improperly defaulted.");
    CHECK_EQ_F(entry2->provider.type, "Unknown",                "provider type was improperly defaulted.");
    CHECK_EQ_F(entry2->provider.url, "",                        "provider url was improperly defaulted.");
    CHECK_EQ_F(entry2->rocket.name, "Unknown",                  "rocket name was improperly defaulted.");
    CHECK_EQ_F(entry2->rocket.family, "Unknown",                "rocket family was improperly defaulted.");
    CHECK_EQ_F(entry2->rocket.variant, "Unknown",               "rocket variant was improperly defaulted.");
    CHECK_EQ_F(entry2->rocket.url, "",                          "rocket url was improperly defaulted.");
    CHECK_EQ_F(entry2->mission.name, "Unknown",                 "mission name was improperly defaulted.");
    CHECK_EQ_F(entry2->mission.description, "Unknown",          "mission description was improperly imported.");
    CHECK_EQ_F(entry2->mission.type, "Unknown",                 "mission type was improperly defaulted.");
    CHECK_EQ_F(entry2->mission.orbit_name, "Unknown",           "mission orbit_name was improperly defaulted.");
    CHECK_EQ_F(entry2->mission.orbit_abbrev, "Unknown",         "mission orbit_abbrev was improperly defaulted.");
    CHECK_EQ_F(entry2->pad.name, "Unknown",                     "pad name was improperly defaulted.");
    CHECK_EQ_F(entry2->pad.url, "",                             "pad url was improperly defaulted.");
    CHECK_EQ_F(entry2->pad.map_url, "",                         "pad map_url was improperly defaulted.");
    CHECK_EQ_F(entry2->pad.latitude, 0.0f,                      "pad latitude was improperly defaulted.");
    CHECK_EQ_F(entry2->pad.longitude, 0.0f,                     "pad longitude was improperly defaulted.");
    CHECK_EQ_F(entry2->pad.location_name, "Unknown",            "pad location_name was improperly defaulted.");
    CHECK_EQ_F(entry2->pad.location_country, "Unknown",         "pad location_country was improperly defaulted.");
    CHECK_EQ_F(entry2->pad.location_url, "",                    "pad location_url was improperly defaulted.");

    auto launchSource3 = R"QERXY(
        {
			"id": "f058ecca-bda7-4797-ae47-b5c450b1bd78",
			"slug": "new-shepard-ns-21",
			"name": "New Shepard | NS-21",
			"status": {
				"id": 2,
				"name": "To Be Determined",
				"abbrev": "TBD"
			},
			"last_updated": "2022-05-18T13:08:58Z",
			"window_end": "2022-05-31T16:30:00Z",
			"window_start": "2022-05-31T13:30:00Z",
			"failreason": "",
			"hashtag": null,
			"launch_service_provider": {
				"id": 141
			},
			"rocket": {
				"id": 7573
			},
			"mission": {
				"id": 6016,
				"launch_designator": null
			},
			"pad": {
				"id": 90,
				"agency_id": 141,
				"info_url": "http://www.blueorigin.com",
				"wiki_url": "https://en.wikipedia.org/wiki/Corn_Ranch",
				"map_image": "https://spacelaunchnow-prod-east.nyc3.digitaloceanspaces.com/media/launch_images/pad_90_20200803143233.jpg",
				"total_launch_count": 21
			},
			"webcast_live": false,
			"image": "https://spacelaunchnow-prod-east.nyc3.digitaloceanspaces.com/media/launcher_images/new2520shepard_image_20190207032624.jpeg",
			"infographic": null,
			"program": []
		}
    )QERXY"_json;


    std::optional<LaunchEntry> entry3 = LaunchEntry::CreateLaunchEntry(launchSource3);
    CHECK_F(entry3.has_value(), "Failed to get the value of entry 3, a even less information attempt.");

    CHECK_EQ_F(entry3->rocket.name, "Unknown",                  "rocket name was improperly defaulted.");
    CHECK_EQ_F(entry3->rocket.family, "Unknown",                "rocket family was improperly defaulted.");
    CHECK_EQ_F(entry3->rocket.variant, "Unknown",               "rocket variant was improperly defaulted.");
    CHECK_EQ_F(entry3->rocket.url, "",                          "rocket url was improperly defaulted.");
    CHECK_EQ_F(entry3->mission.orbit_name, "Unknown",           "mission orbit_name was improperly defaulted.");
    CHECK_EQ_F(entry3->mission.orbit_abbrev, "Unknown",         "mission orbit_abbrev was improperly defaulted.");
    CHECK_EQ_F(entry3->pad.location_name, "Unknown",            "pad location_name was improperly defaulted.");
    CHECK_EQ_F(entry3->pad.location_country, "Unknown",         "pad location_country was improperly defaulted.");
    CHECK_EQ_F(entry3->pad.location_url, "",                    "pad location_url was improperly defaulted.");


    auto launchSource4 = R"QERXY(
        {
			"id": "f058ecca-bda7-4797-ae47-b5c450b1bd78",
			"name": "New Shepard | NS-21",
			"status": {
				"name": "To Be Determined",
				"abbrev": "TBD"
			},
			"last_updated": "2022-05-18T13:08:58Z",
			"window_end": "2022-05-31T16:30:00Z",
			"window_start": "2022-05-31T13:30:00Z"
		}
    )QERXY"_json;
    
    std::optional<LaunchEntry> entry4 = LaunchEntry::CreateLaunchEntry(launchSource4);
    CHECK_F(entry4.has_value(), "Failed to get the value of entry 4, a minimal information attempt.");

    CHECK_EQ_F(entry4->url, "",                                 "url was improperly defaulted.");
    CHECK_EQ_F(entry4->status.description, "Status discripion unavailable", "status description was improperly defaulted.");
    CHECK_EQ_F(entry4->status.net, entry2->status.window_start, "net was improperly defaulted.");
    CHECK_EQ_F(entry4->status.probability, -1,                  "probability was improperly defaulted.");
    CHECK_EQ_F(entry4->status.holdreason, "",                   "holdreason was improperly defaulted.");
    CHECK_EQ_F(entry4->provider.name, "Unknown",                "provider name was improperly defaulted.");
    CHECK_EQ_F(entry4->provider.type, "Unknown",                "provider type was improperly defaulted.");
    CHECK_EQ_F(entry4->provider.url, "",                        "provider url was improperly defaulted.");
    CHECK_EQ_F(entry4->rocket.name, "Unknown",                  "rocket name was improperly defaulted.");
    CHECK_EQ_F(entry4->rocket.family, "Unknown",                "rocket family was improperly defaulted.");
    CHECK_EQ_F(entry4->rocket.variant, "Unknown",               "rocket variant was improperly defaulted.");
    CHECK_EQ_F(entry4->rocket.url, "",                          "rocket url was improperly defaulted.");
    CHECK_EQ_F(entry4->mission.name, "Unknown",                 "mission name was improperly defaulted.");
    CHECK_EQ_F(entry4->mission.description, "Unknown",          "mission description was improperly imported.");
    CHECK_EQ_F(entry4->mission.type, "Unknown",                 "mission type was improperly defaulted.");
    CHECK_EQ_F(entry4->mission.orbit_name, "Unknown",           "mission orbit_name was improperly defaulted.");
    CHECK_EQ_F(entry4->mission.orbit_abbrev, "Unknown",         "mission orbit_abbrev was improperly defaulted.");
    CHECK_EQ_F(entry4->pad.name, "Unknown",                     "pad name was improperly defaulted.");
    CHECK_EQ_F(entry4->pad.url, "",                             "pad url was improperly defaulted.");
    CHECK_EQ_F(entry4->pad.map_url, "",                         "pad map_url was improperly defaulted.");
    CHECK_EQ_F(entry4->pad.latitude, 0.0f,                      "pad latitude was improperly defaulted.");
    CHECK_EQ_F(entry4->pad.longitude, 0.0f,                     "pad longitude was improperly defaulted.");
    CHECK_EQ_F(entry4->pad.location_name, "Unknown",            "pad location_name was improperly defaulted.");
    CHECK_EQ_F(entry4->pad.location_country, "Unknown",         "pad location_country was improperly defaulted.");
    CHECK_EQ_F(entry4->pad.location_url, "",                    "pad location_url was improperly defaulted.");

    //now, test for when Optional return is invalid
    auto launchSource5 = R"QERXY(
        [1,2,3,4,5]
    )QERXY"_json;
    
    std::optional<LaunchEntry> entry5 = LaunchEntry::CreateLaunchEntry(launchSource5);
    CHECK_F(!entry5.has_value(), "Succeeded to get the value of entry 5, which is bad. Its not a JSON object.");

    auto launchSource6 = R"QERXY(
        {
			"name": "New Shepard | NS-21",
			"status": {
				"id": 2,
				"name": "To Be Determined",
				"abbrev": "TBD"
			},
			"last_updated": "2022-05-18T13:08:58Z",
			"window_end": "2022-05-31T16:30:00Z",
			"window_start": "2022-05-31T13:30:00Z"
		}
    )QERXY"_json;
    
    std::optional<LaunchEntry> entry6 = LaunchEntry::CreateLaunchEntry(launchSource6);
    CHECK_F(!entry6.has_value(), "Succeeded to get the value of entry 6, which is bad. It has no UID");

    auto launchSource7 = R"QERXY(
        {
			"id": "f058ecca-bda7-4797-ae47-b5c450b1bd78",
			"status": {
				"id": 2,
				"name": "To Be Determined",
				"abbrev": "TBD"
			},
			"last_updated": "2022-05-18T13:08:58Z",
			"window_end": "2022-05-31T16:30:00Z",
			"window_start": "2022-05-31T13:30:00Z"
		}
    )QERXY"_json;
    
    std::optional<LaunchEntry> entry7 = LaunchEntry::CreateLaunchEntry(launchSource7);
    CHECK_F(!entry7.has_value(), "Succeeded to get the value of entry 7, which is bad. It has no name");

    auto launchSource8 = R"QERXY(
        {
			"id": "f058ecca-bda7-4797-ae47-b5c450b1bd78",
			"name": "New Shepard | NS-21",
			"last_updated": "2022-05-18T13:08:58Z",
			"window_end": "2022-05-31T16:30:00Z",
			"window_start": "2022-05-31T13:30:00Z"
		}
    )QERXY"_json;
    
    std::optional<LaunchEntry> entry8 = LaunchEntry::CreateLaunchEntry(launchSource8);
    CHECK_F(!entry8.has_value(), "Succeeded to get the value of entry 8, which is bad. It has no status");

    //there was an erronious check here (for missing status.name) but we never actually use that. But I am to lazy to renumber the rest.

    auto launchSource10 = R"QERXY(
        {
			"id": "f058ecca-bda7-4797-ae47-b5c450b1bd78",
			"name": "New Shepard | NS-21",
			"status": {
				"id": 2,
				"name": "To Be Determined"
			},
			"last_updated": "2022-05-18T13:08:58Z",
			"window_end": "2022-05-31T16:30:00Z",
			"window_start": "2022-05-31T13:30:00Z"
		}
    )QERXY"_json;
    
    std::optional<LaunchEntry> entry10 = LaunchEntry::CreateLaunchEntry(launchSource10);
    CHECK_F(!entry10.has_value(), "Succeeded to get the value of entry 10, which is bad. It has no status.abbrev");

    auto launchSource11 = R"QERXY(
        {
			"id": "f058ecca-bda7-4797-ae47-b5c450b1bd78",
			"name": "New Shepard | NS-21",
			"status": {
				"id": 2,
				"name": "To Be Determined",
				"abbrev": "TBD"
			},
			"window_end": "2022-05-31T16:30:00Z",
			"window_start": "2022-05-31T13:30:00Z"
		}
    )QERXY"_json;
    
    std::optional<LaunchEntry> entry11 = LaunchEntry::CreateLaunchEntry(launchSource11);
    CHECK_F(!entry11.has_value(), "Succeeded to get the value of entry 11, which is bad. It has no last updated");

    auto launchSource12 = R"QERXY(
        {
			"id": "f058ecca-bda7-4797-ae47-b5c450b1bd78",
			"name": "New Shepard | NS-21",
			"status": {
				"id": 2,
				"name": "To Be Determined",
				"abbrev": "TBD"
			},
			"last_updated": "2022-05-18T13:08:58Z",
			"window_end": "2022-05-31T13:30:00Z"
		}
    )QERXY"_json;
    
    std::optional<LaunchEntry> entry12 = LaunchEntry::CreateLaunchEntry(launchSource12);
    CHECK_F(!entry12.has_value(), "Succeeded to get the value of entry 12, which is bad. It has no window_start");

    auto launchSource13 = R"QERXY(
        {
			"id": "f058ecca-bda7-4797-ae47-b5c450b1bd78",
			"name": "New Shepard | NS-21",
			"status": {
				"id": 2,
				"name": "To Be Determined",
				"abbrev": "TBD"
			},
			"last_updated": "2022-05-18T13:08:58Z",
			"window_start": "2022-05-31T13:30:00Z"
		}
    )QERXY"_json;
    
    std::optional<LaunchEntry> entry13 = LaunchEntry::CreateLaunchEntry(launchSource13);
    CHECK_F(!entry13.has_value(), "Succeeded to get the value of entry 12, which is bad. It has no window_end");

    //now, check for copy, move, and = functions on LaunEntry objects
    LaunchEntry entry14(*entry3);
    CHECK_EQ_F(entry14.uid, entry3->uid, "Copy constructor did not work.");

    LaunchEntry entry15;
    entry15 = entry14;
    CHECK_EQ_F(entry15.uid, entry14.uid, "Assignment operatior did not work");

    LaunchEntry entry16(std::move(entry15));
    CHECK_EQ_F(entry16.uid, entry14.uid, "Move constructor did not work");

    CHECK_EQ_F(str_tolower("LoWerCaSe"), "lowercase", "str_tolower() does not work as expected");
    CHECK_EQ_F(str_tolower("nUmb3rs783"), "numb3rs783", "str_tolower() does not work as expected");

    CHECK_F(entry14.DoesMatchString("shepard"), "A launch entry that contains a string cannot find that string in it!");
    CHECK_F(entry14.DoesMatchString("2022-05-31T13:30:00Z"), "A launch entry that contains a time cannot find that time in it!");
    CHECK_F(!entry14.DoesMatchString("not-present"), "A launch entry that does not contain a string found that string in it!");

    CHECK_F(entry1->DoesFieldMatchString(SortKey::NAME, "Falcon 9"), "A launch entry that contains a string in a specific field cannot find it.");
    CHECK_F(!entry1->DoesFieldMatchString(SortKey::NAME, "fulcon 5"), "A launch entry that does not contains a string in a specific field found it.");
    
    CHECK_F(entry1->DoesFieldMatchString(SortKey::DATE, "2022-05-25T18:27:00Z"), "A launch entry that contains a string in a specific field cannot find it.");
    CHECK_F(!entry1->DoesFieldMatchString(SortKey::DATE, "2022-06-25T18:27:00Z"), "A launch entry that does not contains a string in a specific field found it.");
    
    CHECK_F(entry1->DoesFieldMatchString(SortKey::PROVIDER, "Commercial"), "A launch entry that contains a string in a specific field cannot find it.");
    CHECK_F(!entry1->DoesFieldMatchString(SortKey::PROVIDER, "government"), "A launch entry that does not contains a string in a specific field found it.");
    
    CHECK_F(entry1->DoesFieldMatchString(SortKey::ROCKET, "Block"), "A launch entry that contains a string in a specific field cannot find it.");
    CHECK_F(!entry1->DoesFieldMatchString(SortKey::ROCKET, "Blue Origin"), "A launch entry that does not contains a string in a specific field found it.");
    
    CHECK_F(entry1->DoesFieldMatchString(SortKey::MISSION, "Transporter"), "A launch entry that contains a string in a specific field cannot find it.");
    CHECK_F(!entry1->DoesFieldMatchString(SortKey::MISSION, "Transporter 6"), "A launch entry that does not contains a string in a specific field found it.");
    
    CHECK_F(entry1->DoesFieldMatchString(SortKey::PAD, "Complex 40"), "A launch entry that contains a string in a specific field cannot find it.");
    CHECK_F(!entry1->DoesFieldMatchString(SortKey::PAD, "Spaaace!"), "A launch entry that does not contains a string in a specific field found it.");
    
    CHECK_F(entry1->DoesFieldMatchString(SortKey::LOCATION, "Cape Canaveral"), "A launch entry that contains a string in a specific field cannot find it.");
    CHECK_F(!entry1->DoesFieldMatchString(SortKey::LOCATION, "Florida"), "A launch entry that does not contains a string in a specific field found it.");
    



    LOG_S(INFO) << "LaunchEntry tests Complete";
}
#endif