#include "pch.hpp"
#include "server.hpp"
#include "options.hpp"

#ifdef TESTS_ENABLED
#include "tests.hpp"    
#endif


int main(int argc, char **argv) {
    loguru::init(argc, argv);
    loguru::add_file("everything.log", loguru::Append, loguru::Verbosity_MAX);
    loguru::add_file("latest_readable.log", loguru::Truncate, loguru::Verbosity_INFO);
    loguru::g_stderr_verbosity = 1;

    const Options options(argc, argv);

    #ifdef TESTS_ENABLED
    if (options.runTests){
        return run_tests();
    }
    #endif

    return start_server(options);
}