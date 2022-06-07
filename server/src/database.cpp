#include "pch.hpp"
#include "database.hpp"

/*
Sorting Structs
These structs are for sorting the data in the m_SortedIndices arrays
*/

struct name_compare{
    const std::vector<LaunchEntry>& masterListRef;
    name_compare(const std::vector<LaunchEntry>& mlf)
    : masterListRef(mlf)
    {}

    bool operator()(const uint32_t& a, const uint32_t& b){
        if (a < masterListRef.size() && b < masterListRef.size() ){
            return masterListRef[a].name < masterListRef[b].name;
        }
        return false;
    }
};

struct date_compare{
    const std::vector<LaunchEntry>& masterListRef;
    date_compare(const std::vector<LaunchEntry>& mlf)
    : masterListRef(mlf)
    {}

    bool operator()(const uint32_t& a, const uint32_t& b){
        if (a < masterListRef.size() && b < masterListRef.size() ){
            return masterListRef[a].status.window_start < masterListRef[b].status.window_start;
        }
        return false;
    }
};

struct rocket_compare{
    const std::vector<LaunchEntry>& masterListRef;
    rocket_compare(const std::vector<LaunchEntry>& mlf)
    : masterListRef(mlf)
    {}

    bool operator()(const uint32_t& a, const uint32_t& b){
        if (a < masterListRef.size() && b < masterListRef.size() ){
            return masterListRef[a].rocket.name < masterListRef[b].rocket.name;
        }
        return false;
    }
};

struct provider_compare{
    const std::vector<LaunchEntry>& masterListRef;
    provider_compare(const std::vector<LaunchEntry>& mlf)
    : masterListRef(mlf)
    {}

    bool operator()(const uint32_t& a, const uint32_t& b){
        if (a < masterListRef.size() && b < masterListRef.size() ){
            return masterListRef[a].provider.name < masterListRef[b].provider.name;
        }
        return false;
    }
};
struct mission_compare{
    const std::vector<LaunchEntry>& masterListRef;
    mission_compare(const std::vector<LaunchEntry>& mlf)
    : masterListRef(mlf)
    {}

    bool operator()(const uint32_t& a, const uint32_t& b){
        if (a < masterListRef.size() && b < masterListRef.size() ){
            return masterListRef[a].mission.name < masterListRef[b].mission.name;
        }
        return false;
    }
};
struct pad_compare{
    const std::vector<LaunchEntry>& masterListRef;
    pad_compare(const std::vector<LaunchEntry>& mlf)
    : masterListRef(mlf)
    {}

    bool operator()(const uint32_t& a, const uint32_t& b){
        if (a < masterListRef.size() && b < masterListRef.size() ){
            return masterListRef[a].pad.name < masterListRef[b].pad.name;
        }
        return false;
    }
};
struct location_compare{
    const std::vector<LaunchEntry>& masterListRef;
    location_compare(const std::vector<LaunchEntry>& mlf)
    : masterListRef(mlf)
    {}

    bool operator()(const uint32_t& a, const uint32_t& b){
        if (a < masterListRef.size() && b < masterListRef.size() ){
            return masterListRef[a].pad.location_name < masterListRef[b].pad.location_name;
        }
        return false;
    }
};



Database* Database::s_Singleton = nullptr;



Database::Database(const std::string& path)
: m_Path(path)
{
    //load the file
    std::ifstream databaseFile(path, std::ios::in);
    //this error message is intentionally NOT debug-mode only
    CHECK_F(databaseFile.is_open(), "ERROR: Unable to open the database file.");

    //read into JSON
    nlohmann::json importedJSON;
    databaseFile >> importedJSON;
    
    databaseFile.close();

    //parse the JSON array
    CHECK_F(importedJSON.is_object(), "JSON Database file is in an invalid format.");
    CHECK_F(importedJSON["results"].is_array(), "JSON Database file is in an invalid format.");
    
    for(auto jsonLaunchData : importedJSON["results"]){
        auto entry = LaunchEntry::CreateLaunchEntry(jsonLaunchData);
        if (entry){
            //this line must use std::move to ensure no copying of the entry.
            m_MasterList.push_back(std::move(*entry));
        }
    }
    
    //make sorted lists
    
    //for each possible sort list, fill with std::iota
    for (uint32_t i=0; i<(uint32_t)SortKey::MAXINDEX;i++){
        m_SortedIndices[i].resize(m_MasterList.size(), 0);
        std::iota(m_SortedIndices[i].begin(), m_SortedIndices[i].end(), 0);
    }

    std::stable_sort(
        m_SortedIndices[(uint32_t)SortKey::NAME].begin(), 
        m_SortedIndices[(uint32_t)SortKey::NAME].end(),
        name_compare{m_MasterList}
    );

    std::stable_sort(
        m_SortedIndices[(uint32_t)SortKey::DATE].begin(), 
        m_SortedIndices[(uint32_t)SortKey::DATE].end(),
        date_compare{m_MasterList}
    );

    std::stable_sort(
        m_SortedIndices[(uint32_t)SortKey::ROCKET].begin(), 
        m_SortedIndices[(uint32_t)SortKey::ROCKET].end(),
        rocket_compare{m_MasterList}
    );

    std::stable_sort(
        m_SortedIndices[(uint32_t)SortKey::PROVIDER].begin(), 
        m_SortedIndices[(uint32_t)SortKey::PROVIDER].end(),
        provider_compare{m_MasterList}
    );

    std::stable_sort(
        m_SortedIndices[(uint32_t)SortKey::MISSION].begin(), 
        m_SortedIndices[(uint32_t)SortKey::MISSION].end(),
        mission_compare{m_MasterList}
    );

    std::stable_sort(
        m_SortedIndices[(uint32_t)SortKey::PAD].begin(), 
        m_SortedIndices[(uint32_t)SortKey::PAD].end(),
        pad_compare{m_MasterList}
    );

    std::stable_sort(
        m_SortedIndices[(uint32_t)SortKey::LOCATION].begin(), 
        m_SortedIndices[(uint32_t)SortKey::LOCATION].end(),
        location_compare{m_MasterList}
    );
}

Database* Database::Get(){
    DCHECK_NOTNULL_F(Database::s_Singleton, "ERROR: The database was not initialized before Database::Get() was called, or, it was Destroyed.");
    return s_Singleton;
}

Database* Database::Init(const std::string& path){
    DCHECK_EQ_F(Database::s_Singleton, nullptr, "ERROR: the database cannot be initialized twice!");
    Database::s_Singleton = new Database(path);
    LOG_S(INFO) << "Database Created!";
    return Database::s_Singleton;
}

void Database::Destroy(){
    DCHECK_NOTNULL_F(Database::s_Singleton, "ERROR: The database was not initialized before Database::Delete() was called.");
    LOG_S(INFO) << "Database Destroyed!";
    delete s_Singleton;
}


const LaunchEntry& Database::GetEntryFromMaster(uint32_t index) const{
    DCHECK_F(index < m_MasterList.size(), "Index passed to GetEntryFromMaster must be less than masterlist size");
    return m_MasterList[index];
}


size_t Database::GetMasterListSize() const{
    return m_MasterList.size();
}


const std::vector<uint32_t>& Database::GetSortedIndex(SortKey key) const{
    return m_SortedIndices[(size_t)key];
}


std::string Database::DumpMasterListJSON(){
    nlohmann::json jsonList = nlohmann::json::object();
    jsonList["results"] = nlohmann::json::array();
    jsonList["count"] = m_MasterList.size();

    for (auto& elm : m_MasterList){
        jsonList["results"].push_back(elm.ToJSON());
    }

    std::stringstream ss;
    ss << jsonList.dump(4);
    return ss.str();
}


#ifdef TESTS_ENABLED
void database_tests(){
    LOG_S(INFO) << "Starting Database Tests";

    Database* db = Database::Get();
    
    auto& sortedName = db->GetSortedIndex(SortKey::NAME);
    int32_t prev = -1;
    for (auto i : sortedName){
        if (prev > -1){
            CHECK_F(
                (db->GetEntryFromMaster(prev).name <= db->GetEntryFromMaster(i).name), 
                "Sort FAILED. Names are not in order: %s, %s",
                db->GetEntryFromMaster(prev).name.c_str(),
                db->GetEntryFromMaster(i).name.c_str()
            );
        }
        prev = i;
    }
    LOG_S(INFO) << "Database Name Sort checks complete. All good!";

    auto& sortedDate = db->GetSortedIndex(SortKey::DATE);
    prev = -1;
    for (auto i : sortedDate){
        if (prev > -1){
            CHECK_F(
                (db->GetEntryFromMaster(prev).status.window_start <= db->GetEntryFromMaster(i).status.window_start), 
                "Sort FAILED. dates are not in order: %s, %s",
                TimeToText(db->GetEntryFromMaster(prev).status.window_start).c_str(),
                TimeToText(db->GetEntryFromMaster(i).status.window_start).c_str()
            );
        }
        prev = i;
    }
    LOG_S(INFO) << "Database Date Sort checks complete. All good!";

    auto& sortedRocket = db->GetSortedIndex(SortKey::ROCKET);
    prev = -1;
    for (auto i : sortedRocket){
        if (prev > -1){
            CHECK_F(
                (db->GetEntryFromMaster(prev).rocket.name <= db->GetEntryFromMaster(i).rocket.name), 
                "Sort FAILED. Rocket names are not in order: %s, %s",
                db->GetEntryFromMaster(prev).rocket.name.c_str(),
                db->GetEntryFromMaster(i).rocket.name.c_str()
            );
        }
        prev = i;
    }
    LOG_S(INFO) << "Database Rocket Name Sort checks complete. All good!";

    auto& sortedProvider = db->GetSortedIndex(SortKey::PROVIDER);
    prev = -1;
    for (auto i : sortedProvider){
        if (prev > -1){
            CHECK_F(
                (db->GetEntryFromMaster(prev).provider.name <= db->GetEntryFromMaster(i).provider.name), 
                "Sort FAILED. Provider names are not in order: %s, %s",
                db->GetEntryFromMaster(prev).provider.name.c_str(),
                db->GetEntryFromMaster(i).provider.name.c_str()
            );
        }
        prev = i;
    }
    LOG_S(INFO) << "Database Provider Name Sort checks complete. All good!";

    auto& sortedMission = db->GetSortedIndex(SortKey::MISSION);
    prev = -1;
    for (auto i : sortedMission){
        if (prev > -1){
            CHECK_F(
                (db->GetEntryFromMaster(prev).mission.name <= db->GetEntryFromMaster(i).mission.name), 
                "Sort FAILED. Mission names are not in order: %s, %s",
                db->GetEntryFromMaster(prev).mission.name.c_str(),
                db->GetEntryFromMaster(i).mission.name.c_str()
            );
        }
        prev = i;
    }
    LOG_S(INFO) << "Database Mission Name Sort checks complete. All good!";

    auto& sortedPad = db->GetSortedIndex(SortKey::PAD);
    prev = -1;
    for (auto i : sortedPad){
        if (prev > -1){
            CHECK_F(
                (db->GetEntryFromMaster(prev).pad.name <= db->GetEntryFromMaster(i).pad.name), 
                "Sort FAILED. Pad names are not in order: %s, %s",
                db->GetEntryFromMaster(prev).pad.name.c_str(),
                db->GetEntryFromMaster(i).pad.name.c_str()
            );
        }
        prev = i;
    }
    LOG_S(INFO) << "Database Pad Name Sort checks complete. All good!";

    auto& sortedLocation = db->GetSortedIndex(SortKey::LOCATION);
    prev = -1;
    for (auto i : sortedLocation){
        if (prev > -1){
            CHECK_F(
                (db->GetEntryFromMaster(prev).pad.location_name <= db->GetEntryFromMaster(i).pad.location_name), 
                "Sort FAILED. Location names are not in order: %s, %s",
                db->GetEntryFromMaster(prev).pad.location_name.c_str(),
                db->GetEntryFromMaster(i).pad.location_name.c_str()
            );
        }
        prev = i;
    }
    LOG_S(INFO) << "Database Location Name Sort checks complete. All good!";

    LOG_S(INFO) << "Database Tests complete";
}
#endif
