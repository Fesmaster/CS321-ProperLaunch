#pragma once
#include "launch_entry.hpp"
#include "parseutm.hpp"


class Database{

public:
    /*
    get the pre-created instance of the database. 
    raises an error if Database::Init has not been called.
    */
    static Database* Get();

    /*
    Initialize the database, performing initial load and sort
    */
    static Database* Init(const std::string& path);

    /*
    Destroy the database. Must be re-initialized if you want to use it again.
    Failing to call this at program's end will result in a memory leak.
    */
    static void Destroy();

    /*
    Get a launch entry from the master list
    */
    const LaunchEntry& GetEntryFromMaster(uint32_t index) const;

    /*
    Get the size of the Master List
    */
    size_t GetMasterListSize() const;

    /*
    Get a sorted list
    */
    const std::vector<uint32_t>& GetSortedIndex(SortKey key) const;

    /*
    TESTING ONLY, to REMOVE. Or, use as template for functions that will reply when the server is queried.
    */
    std::string DumpMasterListJSON();
    
    //function deletion (to maintain singleton status)
    Database(const Database& other) = delete;
    Database(Database&& other) = delete;
    Database operator= (const Database& other) = delete;
private:
    static Database* s_Singleton;
    
    //private constructor
    Database(const std::string& path);
    

    std::string m_Path;
    std::vector<LaunchEntry> m_MasterList;
    std::array<std::vector<uint32_t>, (size_t)SortKey::MAXINDEX> m_SortedIndices; 
};


#ifdef TESTS_ENABLED
void database_tests();
#endif
