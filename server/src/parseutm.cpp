#include "pch.hpp"
#include <regex>
#include "parseutm.hpp"
#include "tests.hpp"

/*
 * Parses the end of the UTM of the format <original>?SORT=<sort-type-key>&FILTER=<filter-type-key>&RANGE=<range>
 * Filters consist of a filter type and a range (i.e. FILTER=LOCATION&RANGE=IDAHO)
 *
 * Sort/filter string keys need to be entered exactly as they are spelled (LOCATION, Location, and location will all work as long as they are spelled correctly)
 *
 * Only filter and range need to accompany one another with range placed after filter
 * a filter can be entered without a sort and a default sort type will be assigned
 * Absence of both a sort and filter will result in a default sort type and no filter type 
 *
 * '&' is used to mark the end of a sort, filter, and range key if they are not at the end of the string
 * '&' and '%' can not be entered as a search characters on their own so '%1' has to be used for '&' and '%2' for '%'
 *
 * sort/filter statements and keys are case insensitive
 * the filter range statement is case insensitive and its key is a string taken literally, including a blank string
 * soRt=lOcation&fIlter=loCatiOn&raNge=iDaho will work
 *
 * filterstyle is set to MAXINDEX by default
 */

utmdata::utmdata(const std::string& utmstr)
	: sortstyle(SortKey::DATE), filterstyle(SortKey::MAXINDEX), filterrange("")
{
	COVERAGE_BRANCH
	if (!utmstr.empty()){
		COVERAGE_BRANCH

		/* log: constructor argument */
		LOG_S(9) << "CONSTRUCTOR: " << utmstr;

		/* vector and iterator for sort/filter types */ 
		std::vector<std::string> strvec{"NAME", "DATE", "ROCKET", "PROVIDER", "MISSION", "PAD", "LOCATION"};
		std::vector<std::string>::iterator it;

		/* Copy of UTM string */
		std::string str = utmstr;
		std::string strres;

		/* change attributes to lowercase(attributes i.e. sort=) */
		str = std::regex_replace(str, std::regex("sort=?(?=(.*(&|$)))", std::regex_constants::icase), "sort=");
		str = std::regex_replace(str, std::regex("filter=?(?=(.*(&|$)))", std::regex_constants::icase), "filter=");
		str = std::regex_replace(str, std::regex("range=?(?=(.*(&|$)))", std::regex_constants::icase), "range=");

		std::smatch sm;

		/* regex pattern for sort, will give "sort=<sortstyle>" */
		std::regex_search(str, sm, std::regex("(?=(sort=)).*?(?=(&|$))"));


		int j = 0;

		if(sm.size()>0){	
			COVERAGE_BRANCH
			/* get sort type */
			strres = sm.str();

			/* remove sortsyle from string (the string after "sort=") */
			strres = strres.substr(strres.find("=")+1);

			/* change to upper case */
			transform(strres.begin(), strres.end(), strres.begin(), ::toupper);

			for(it = strvec.begin(), j=0; it < strvec.end(); it++, j++){
				if(strres == *it){
					COVERAGE_BRANCH
					sortstyle = (SortKey)j;

					/* log: setting sortstyle */
					LOG_S(2) << "Sort type match found in list for " << strres << "\nSet sortstyle key: " << j;
					break;
				}
				COVERAGE_BRANCH_ELSE
			}

			if(it >= strvec.end()){
				COVERAGE_BRANCH
				LOG_S(1) << "No sort type match found in list for " << strres;
			}
			COVERAGE_BRANCH_ELSE

		}
		else{
			COVERAGE_BRANCH
			LOG_S(1) << "No sort statement found";
		}

		/* regex pattern for filter=<filterstyle>&range=<range> */
		std::regex_search(str, sm, std::regex("(?=(filter=)).*?&range=.*?(?=(&|$))"));

		/* Check if complete filter/range statement exists */
		if(sm.size()>0){
			COVERAGE_BRANCH
			j = 0;

			strres = sm.str();

			std::string strres2;

			/* regex pattern for filter=<filterstyle */
			std::regex_search(strres, sm, std::regex(".*?(?=&)"));

			strres2 = sm.str();

			/* retreave filterstyle */
			strres2 = strres2.substr(strres2.find("=")+1);

			/* uppercase */
			transform(strres2.begin(), strres2.end(), strres2.begin(), ::toupper);

			for(j = 0, it = strvec.begin(); it < strvec.end(); it++, j++){
				if(strres2 == *it){
					COVERAGE_BRANCH
					filterstyle = (SortKey)j;
					break;
				}
				COVERAGE_BRANCH_ELSE
			}
			if(it == strvec.end()){
				COVERAGE_BRANCH
				/* log: no match to SortKey enum */
				LOG_S(1) << "No filter type match found in list for " << strres2;
			}
			else{
				COVERAGE_BRANCH
				/* log: set filterstyle */
				LOG_S(2) << "Filter type match found in list for " << strres2 << "\nSet filterstyle key: " << j;

				/* regex pattern for range=<range> */
				std::regex_search(strres, sm, std::regex("(?=range=).*?(?=(&|$))"));

				strres2 = sm.str();

				/* retrieve range */
				strres2 = strres2.substr(strres2.find("=")+1);

				/* replace escape patterns */
				strres2 = std::regex_replace(strres2, std::regex("%1"), "&");
				strres2 = std::regex_replace(strres2, std::regex("%2"), "%");

				filterrange += strres2;

				/* log: set filterrange to string */
				LOG_S(2) << "Set filterrange: " << strres2;
			}

		}
		else{
			COVERAGE_BRANCH
			LOG_S(1) <<  "No filter/range statement found in string";
		}
	}
	COVERAGE_BRANCH_ELSE

}

#ifdef TESTS_ENABLED
void utm_tests()
{
	LOG_S(INFO) << "UTM Tests begin";

	utmdata d1("/?sort=NAME&filter=PROVIDER&range=spacex");
	CHECK_EQ_F(d1.sortstyle, SortKey::NAME, "parsed UTM data does not have the correct sort style");
	CHECK_EQ_F(d1.filterstyle, SortKey::PROVIDER, "parsed UTM data does not have the correct filter style");
	CHECK_EQ_F(d1.filterrange, "spacex", "parsed UTM data does not have the correct filter range: [%s]", d1.filterrange.c_str());


	utmdata d2("/?something wacky here **(&^(*^&^R*&^%()*&%^");
	CHECK_EQ_F(d2.sortstyle, SortKey::DATE, "parsed UTM data does not have the correct default sort style");
	CHECK_EQ_F(d2.filterstyle, SortKey::MAXINDEX, "parsed UTM data does not have the correct default filter style");
	CHECK_EQ_F(d2.filterrange, "", "parsed UTM data does not have the correct default filter range: [%s]", d2.filterrange.c_str());

	utmdata d3("/?sort=NAME");
	CHECK_EQ_F(d3.sortstyle, SortKey::NAME, "parsed UTM data does not have the correct sort style");
	CHECK_EQ_F(d3.filterstyle, SortKey::MAXINDEX, "parsed UTM data does not have the correct default filter style");
	CHECK_EQ_F(d3.filterrange, "", "parsed UTM data does not have the correct filter range: [%s]", d3.filterrange.c_str());

	utmdata d4("/?sort=FUNKY");
	CHECK_EQ_F(d4.sortstyle, SortKey::DATE, "parsed UTM data does not have the correct sort style");
	CHECK_EQ_F(d4.filterstyle, SortKey::MAXINDEX, "parsed UTM data does not have the correct default filter style");
	CHECK_EQ_F(d4.filterrange, "", "parsed UTM data does not have the correct default filter range: [%s]", d4.filterrange.c_str());

	utmdata d5("/?filter=PROVIDER&range=spacex");
	CHECK_EQ_F(d5.sortstyle, SortKey::DATE, "parsed UTM data does not have the correct default sort style");
	CHECK_EQ_F(d5.filterstyle, SortKey::PROVIDER, "parsed UTM data does not have the correct filter style");
	CHECK_EQ_F(d5.filterrange, "spacex", "parsed UTM data does not have the correct filter range: [%s]", d5.filterrange.c_str());

	utmdata d6("/?filter=FUNKEY&range=spacex");
	CHECK_EQ_F(d6.sortstyle, SortKey::DATE, "parsed UTM data does not have the correct default sort style");
	CHECK_EQ_F(d6.filterstyle, SortKey::MAXINDEX, "parsed UTM data does not have the correct filter style");
	CHECK_EQ_F(d6.filterrange, "", "parsed UTM data does not have the correct default filter range: [%s]", d6.filterrange.c_str());
	
	LOG_S(INFO) << "UTM Tests complete";
}
#endif
