#include "pch.hpp"
#include "server.hpp"
#include "options.hpp"
#include "database.hpp"


#include "tests.hpp"    



int main(int argc, char **argv) {
    loguru::init(argc, argv);
    loguru::add_file("everything.log", loguru::Append, loguru::Verbosity_MAX);
    loguru::add_file("latest_readable.log", loguru::Truncate, loguru::Verbosity_INFO);
    loguru::g_stderr_verbosity = 1;

    const Options options(argc, argv);

    //init the database
    Database::Init(options.sourcePath);

    #ifdef TESTS_ENABLED
    if (options.runTests){
        auto rval = run_tests();
        Database::Destroy();
        return rval;
    }
    #endif

    auto rval = start_server(options);
    Database::Destroy();
    return rval;
}