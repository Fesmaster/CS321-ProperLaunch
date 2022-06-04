#include "pch.hpp"
#ifdef TESTS_ENABLED
#include "tests.hpp"

#include "options.hpp"
#include "launch_entry.hpp"


int run_tests(){
    LOG_S(INFO) << "TESTS ARE RUNNING";
    //all the test functions should be called here    
    option_tests();
    launch_entry_tests();

    return 0;
}




#endif