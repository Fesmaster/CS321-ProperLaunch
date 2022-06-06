#include "pch.hpp"
#include "database.hpp"

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
}

Database* Database::Get(){
    DCHECK_NOTNULL_F(Database::s_Singleton, "ERROR: The database was not initialized before Database::Get() was called, or, it was Destroyed.");
    return s_Singleton;
}

Database* Database::Init(const std::string& path){
    DCHECK_EQ_F(Database::s_Singleton, nullptr, "ERROR: the database cannot be initialized twice!");
    Database::s_Singleton = new Database(path);
    return Database::s_Singleton;
}

void Database::Destroy(){
    DCHECK_NOTNULL_F(Database::s_Singleton, "ERROR: The database was not initialized before Database::Delete() was called.");
    delete s_Singleton;
}



std::string Database::DumpMasterListJSON()
{
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


    LOG_S(INFO) << "Database Tests complete";
}
#endif
