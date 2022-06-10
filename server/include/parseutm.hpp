#pragma once
/*
 * Parses the end of the UTM of the formate <original>?<"SORT="-or-"FILTER="><type-of-sort-or-filter>&<remaining-"SORT="-or-"FILTER="><type-of-sort-or-filter>
 * Sort or filter can be chosen on their own, together, or not at all
 *
 * These two are also valid
 * <original>?<"SORT="-or-"FILTER="><type-of-sort-or-filter>
 * <original>?
 *
 * '&' is used to join a sort and a filter
 * '&' can not be entered as a search character on its own so '%' has to be used as an escape character (i.e. '%&')
 */

#include <iostream>
#include <string>
#include <cstring>

enum class SortKey : uint32_t{
    NAME=0, 
    DATE, 
    ROCKET, 
    PROVIDER, 
    MISSION, 
    PAD, 
    LOCATION, 
    MAXINDEX //simply for looping over all these values
};

struct utmdata{
	std::string filterstyle;
	SortKey sortstyle;

	utmdata(const std::string& utmstr);
};


#ifdef TESTS_ENABLED
void utm_tests();
#endif