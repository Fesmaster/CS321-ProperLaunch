#pragma once
#include "pch.hpp"

#ifdef TESTS_ENABLED
/*
This function runs all the tests
*/
int run_tests();

void coverage_marker(const std::string& file, const std::string& func, int line);


#define COVERAGE_BRANCH coverage_marker(__FILE__, __FUNCTION__, __LINE__);
#define COVERAGE_BRANCH_ELSE else{coverage_marker(__FILE__, __FUNCTION__, __LINE__);}
#define COVERAGE_NEVER ABORT_F("A (redundant) statment that should not be executed was. File: %s  Function: %s  Line: %i", __FILE__, __FUNCTION__, __LINE__);
#define COVERAGE_NEVER_ELSE else{ABORT_F("A (redundant) statment that should not be executed was. File: %s  Function: %s  Line: %i", __FILE__, __FUNCTION__, __LINE__);}

#else
#define COVERAGE_BRANCH
#define COVERAGE_BRANCH_ELSE
#endif