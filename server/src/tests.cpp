#include "pch.hpp"
#ifdef TESTS_ENABLED
#include "tests.hpp"

#include "options.hpp"
#include "launch_entry.hpp"
#include "database.hpp"

static std::unordered_set<std::string> s_FoundBranches;

int run_tests(){
    LOG_S(INFO) << "TESTS ARE RUNNING";
    //all the test functions should be called here    
    option_tests();
    launch_entry_tests();
    database_tests();

    LOG_S(INFO) << "Branches Run: " << s_FoundBranches.size() << " of " << T_BRANCH_COUNT << "(" << ((float)s_FoundBranches.size()/(float)T_BRANCH_COUNT*100 ) << "%)";
    return 0;
}



void coverage_marker(const std::string& file, const std::string& func, int line){
    std::stringstream fullname;
    fullname << file << '#' << func << "#" << line;
    s_FoundBranches.insert(fullname.str());
}


#endif