#pragma once
#include "pch.hpp"

//this will probably expand
enum class LaunchCode{
    CONFIRMED, 
    UNCONFIRMED, 
    UNDETERMINED,
    HELD,
};

struct LaunchStatus{
    LaunchCode code;
    std::string description;
    std::chrono::time_point<std::chrono::system_clock> last_updated;
    std::chrono::time_point<std::chrono::system_clock> net;
    std::chrono::time_point<std::chrono::system_clock> window_start;
    std::chrono::time_point<std::chrono::system_clock> window_end;
    int probability;
    std::string holdreason;
    
    LaunchStatus();
    LaunchStatus(const LaunchStatus& other);
    LaunchStatus& operator=(const LaunchStatus& other);
    LaunchStatus(LaunchStatus&& other);

};

struct LaunchProvider{
    std::string name;
    std::string type;
    std::string url;

    LaunchProvider();
    LaunchProvider(const LaunchProvider& other);
    LaunchProvider& operator=(const LaunchProvider& other);
    LaunchProvider(LaunchProvider&& other);
};

struct LaunchVessel {
    std::string name;
    std::string family;
    std::string variant;
    std::string url;

    LaunchVessel();
    LaunchVessel(const LaunchVessel& other);
    LaunchVessel& operator=(const LaunchVessel& other);
    LaunchVessel(LaunchVessel&& other);
};

struct LaunchMission {
    std::string name;
    std::string description;
    std::string type;
    std::string orbit_name;
    std::string orbit_abbrev;

    LaunchMission();
    LaunchMission(const LaunchMission& other);
    LaunchMission& operator=(const LaunchMission& other);
    LaunchMission(LaunchMission&& other);
};

struct LaunchPad {
    std::string name;
    std::string url;
    std::string map_url;
    float latitude;
    float longitude;
    std::string location_url;
    std::string location_name;
    std::string location_country;

    LaunchPad();
    LaunchPad(const LaunchPad& other);
    LaunchPad& operator=(const LaunchPad& other);
    LaunchPad(LaunchPad&& other);
};

/*
This struct is designed to hold all the data about a particular launch.
Contains the ability to build one from TheSpaceDevs JSON format, and
build JSON of our format
*/
struct LaunchEntry{
    std::string uid;
    std::string name;
    std::string url;
    LaunchStatus status;
    LaunchProvider provider;
    LaunchVessel rocket;
    LaunchMission mission;
    LaunchPad pad;

    /*CONSTRUCTORS*/
    LaunchEntry();
    LaunchEntry(const LaunchEntry& other);
    LaunchEntry& operator=(const LaunchEntry& other);
    LaunchEntry(LaunchEntry&& other);

    /*
    Create a new LaunchEntity struct by parsing a json object from TheSpaceDevs
    the return is wrapped in std::optional because there is a chance it will
    fail, if the JSON is not the expected data
    */
    static std::optional<LaunchEntry> CreateLaunchEntry(nlohmann::json json);

    /*
    Create a JSON object of our format from the particular LaunchData.
    */
    nlohmann::json ToJSON();
};
