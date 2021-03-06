#include "pch.hpp"
#include "database.hpp"
#include "tests.hpp"

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
        COVERAGE_BRANCH
        if (a < masterListRef.size() && b < masterListRef.size() ){
            COVERAGE_BRANCH
            return masterListRef[a].name < masterListRef[b].name;
        }
        COVERAGE_NEVER_ELSE
        return false;
    }
};

struct date_compare{
    const std::vector<LaunchEntry>& masterListRef;
    date_compare(const std::vector<LaunchEntry>& mlf)
    : masterListRef(mlf)
    {}

    bool operator()(const uint32_t& a, const uint32_t& b){
        COVERAGE_BRANCH
        if (a < masterListRef.size() && b < masterListRef.size() ){
            COVERAGE_BRANCH
            return masterListRef[a].status.window_start < masterListRef[b].status.window_start;
        }
        COVERAGE_NEVER_ELSE
        return false;
    }
};

struct rocket_compare{
    const std::vector<LaunchEntry>& masterListRef;
    rocket_compare(const std::vector<LaunchEntry>& mlf)
    : masterListRef(mlf)
    {}

    bool operator()(const uint32_t& a, const uint32_t& b){
        COVERAGE_BRANCH
        if (a < masterListRef.size() && b < masterListRef.size() ){
            COVERAGE_BRANCH
            return masterListRef[a].rocket.name < masterListRef[b].rocket.name;
        }
        COVERAGE_NEVER_ELSE
        return false;
    }
};

struct provider_compare{
    const std::vector<LaunchEntry>& masterListRef;
    provider_compare(const std::vector<LaunchEntry>& mlf)
    : masterListRef(mlf)
    {}

    bool operator()(const uint32_t& a, const uint32_t& b){
        COVERAGE_BRANCH
        if (a < masterListRef.size() && b < masterListRef.size() ){
            COVERAGE_BRANCH
            return masterListRef[a].provider.name < masterListRef[b].provider.name;
        }
        COVERAGE_NEVER_ELSE
        return false;
    }
};
struct mission_compare{
    const std::vector<LaunchEntry>& masterListRef;
    mission_compare(const std::vector<LaunchEntry>& mlf)
    : masterListRef(mlf)
    {}

    bool operator()(const uint32_t& a, const uint32_t& b){
        COVERAGE_BRANCH
        if (a < masterListRef.size() && b < masterListRef.size() ){
            COVERAGE_BRANCH
            return masterListRef[a].mission.name < masterListRef[b].mission.name;
        }
        COVERAGE_NEVER_ELSE
        return false;
    }
};
struct pad_compare{
    const std::vector<LaunchEntry>& masterListRef;
    pad_compare(const std::vector<LaunchEntry>& mlf)
    : masterListRef(mlf)
    {}

    bool operator()(const uint32_t& a, const uint32_t& b){
        COVERAGE_BRANCH
        if (a < masterListRef.size() && b < masterListRef.size() ){
            COVERAGE_BRANCH
            return masterListRef[a].pad.name < masterListRef[b].pad.name;
        }
        COVERAGE_NEVER_ELSE
        return false;
    }
};
struct location_compare{
    const std::vector<LaunchEntry>& masterListRef;
    location_compare(const std::vector<LaunchEntry>& mlf)
    : masterListRef(mlf)
    {}

    bool operator()(const uint32_t& a, const uint32_t& b){
        COVERAGE_BRANCH
        if (a < masterListRef.size() && b < masterListRef.size() ){
            COVERAGE_BRANCH
            return masterListRef[a].pad.location_name < masterListRef[b].pad.location_name;
        }
        COVERAGE_NEVER_ELSE
        return false;
    }
};



Database* Database::s_Singleton = nullptr;



Database::Database(const std::string& path)
: m_Path(path)
{
    COVERAGE_BRANCH
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
            COVERAGE_BRANCH
            //this line must use std::move to ensure no copying of the entry.
            m_MasterList.push_back(std::move(*entry));
        }
        COVERAGE_NEVER_ELSE //This one will likely never be run. It is nesecary because it is part of the data parsing system.
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

    lastUpdate = std::chrono::system_clock::now();
}

Database* Database::Get(){
    COVERAGE_BRANCH
    DCHECK_NOTNULL_F(Database::s_Singleton, "ERROR: The database was not initialized before Database::Get() was called, or, it was Destroyed.");
    return s_Singleton;
}

Database* Database::Init(const std::string& path){
    COVERAGE_BRANCH
    DCHECK_EQ_F(Database::s_Singleton, nullptr, "ERROR: the database cannot be initialized twice!");
    Database::s_Singleton = new Database(path);
    LOG_S(INFO) << "Database Created!";
    return Database::s_Singleton;
}

void Database::Destroy(){
    //COVERAGE BRANCH //this will never get coverage as its destroyed after coverage is measured.
    DCHECK_NOTNULL_F(Database::s_Singleton, "ERROR: The database was not initialized before Database::Delete() was called.");
    LOG_S(INFO) << "Database Destroyed!";
    delete s_Singleton;
}


const LaunchEntry& Database::GetEntryFromMaster(uint32_t index) const{
    COVERAGE_BRANCH
    DCHECK_F(index < m_MasterList.size(), "Index passed to GetEntryFromMaster must be less than masterlist size");
    return m_MasterList[index];
}


size_t Database::GetMasterListSize() const{
    COVERAGE_BRANCH
    return m_MasterList.size();
}


const std::vector<uint32_t>& Database::GetSortedIndex(SortKey key) const{
    COVERAGE_BRANCH
    DCHECK_F((uint32_t)key < (uint32_t)SortKey::MAXINDEX, "You cannot use values of SortKey::MAXINDEX or greater for a sort index. Its for iterating only!");
    return m_SortedIndices[(size_t)key];
}


nlohmann::json Database::GetFilteredList(const utmdata& query) const{
    COVERAGE_BRANCH
    DCHECK_F((uint32_t)query.sortstyle < (uint32_t)SortKey::MAXINDEX, "You cannot use values of SortKey::MAXINDEX or greater for a sort index. Its for iterating only!");
    auto& list = m_SortedIndices[(size_t)query.sortstyle];
    std::vector<nlohmann::json> jsonVector;
    uint32_t count = 0;
    for(auto index : list){
        auto& entry = m_MasterList[index];
        if (query.filterstyle == SortKey::MAXINDEX){
            COVERAGE_BRANCH
            if (entry.DoesMatchString(query.filterrange)){
                COVERAGE_BRANCH
                jsonVector.push_back(entry.ToJSON());
            }
            COVERAGE_BRANCH_ELSE
        }
        else{
            COVERAGE_BRANCH
            if (entry.DoesFieldMatchString(query.filterstyle, query.filterrange)){
                COVERAGE_BRANCH
                jsonVector.push_back(entry.ToJSON());
            }
            COVERAGE_BRANCH_ELSE
        }
    }

    return nlohmann::json{
        {"count", jsonVector.size()},
        {"results", nlohmann::json(jsonVector)}
    };
}


std::string Database::DumpMasterListJSON(){
    //COVERAGE BRANCH //this is not covered in coverage tests as it is dev only and will be removed later
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


void Database::CheckDatabaseUpdate(){
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(
        std::chrono::system_clock::now() - lastUpdate
    );
    if (minutes.count() > 30){
        LOG_S(INFO) << "Scheduling Async Database update";
        updateResult = std::async(std::launch::async, [](){
            LOG_S(INFO) << "Performing Async Database update";
            Database* db = Database::Get();
            db->UpdateData();
            db->lastUpdate = std::chrono::system_clock::now();
            LOG_S(INFO) << "Completing Async Database update";
        });
    }
}


void Database::UpdateData(){
    //load the file
    std::ifstream databaseFile(m_Path, std::ios::in);
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


void Database::WaitDatabaseUpdate(){
    if (updateResult.valid()){
        updateResult.wait();
        LOG_S(INFO) << "Async Database update wait completed";
    }
    return;
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

    utmdata d("");
    d.filterrange = "Jeb";
    auto results1 = db->GetFilteredList(d);
    CHECK_F( results1["count"] == 2, "Count was not correct for testing data filtered with \"Jeb\"! Did you remember to use the testing dataset (-p ./testubgdata,json)?");

    d.filterstyle = SortKey::PROVIDER;
    d.filterrange = "Lowne Aerospace";
    auto results2 = db->GetFilteredList(d);
    CHECK_F( results2["count"] == 1, "Count was not correct for testing data filtered with \"Lowne Aerospace\"! Did you remember to use the testing dataset (-p ./testubgdata,json)?");



    LOG_S(INFO) << "Database Tests complete";
}
#endif
