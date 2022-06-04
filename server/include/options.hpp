#pragma once
#include "pch.hpp"
struct Options{
    Options(int argc, char** argv);

    std::string sourcePath;
    std::string port;
    
    #ifdef TESTS_ENABLED
    bool runTests = false;
    #endif

};

#ifdef TESTS_ENABLED
void option_tests();
#endif