#pragma once

/* 
 * Header file with struct to store sort/filter/range from UTM
 *
 * <original-url>/?SORT=[sortstyle]&FILTER=[filterstyle]&RANGE=[filterrange]
 *
 * sort and filter keys are both Sortkey : uint32_t
 * range key is a string(i.e. "IDAHO") literally as it was inputted
 *
 */

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
	SortKey sortstyle;
	SortKey filterstyle;
	std::string filterrange;
	utmdata(const std::string& utmstr);
};

#ifdef TESTS_ENABLED
void utm_tests();
#endif
