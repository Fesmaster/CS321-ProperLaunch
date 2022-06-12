#include <regex>
#include "pch.hpp"
#include "parseutm.hpp"

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
	if (!utmstr.empty()){

		/* log: constructor argument */
		LOG_S(INFO) << "CONSTRUCTOR: " << utmstr;

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

		if(sm.size()>0)
		{	
			/* get sort type */
			strres = sm.str();

			/* remove sortsyle from string (the string after "sort=") */
			strres = strres.substr(strres.find("=")+1);

			/* change to upper case */
			transform(strres.begin(), strres.end(), strres.begin(), ::toupper);

			for(it = strvec.begin(), j=0; it < strvec.end(); it++, j++)
			{
				if(strres == *it)
				{
					sortstyle = (SortKey)j;

					/* log: setting sortstyle */
					LOG_S(INFO) << "Sort type match found in list for " << strres << "\nSet sortstyle key: " << j;
					break;
				}
			}

			if(it >= strvec.end())
			{
				LOG_S(INFO) << "No sort type match found in list for " << strres;
			}

		}
		else
		{
			LOG_S(INFO) << "No sort statement found";
		}

		/* regex pattern for filter=<filterstyle>&range=<range> */
		std::regex_search(str, sm, std::regex("(?=(filter=)).*?&range=.*?(?=(&|$))"));

		/* Check if complete filter/range statement exists */
		if(sm.size()>0)
		{
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

			for(j = 0, it = strvec.begin(); it < strvec.end(); it++, j++)
			{
				if(strres2 == *it)
				{
					filterstyle = (SortKey)j;
					break;
				}
			}
			if(it == strvec.end())
			{
				/* log: no match to SortKey enum */
				LOG_S(INFO) << "No filter type match found in list for " << strres2;
			}
			else
			{
				/* log: set filterstyle */
				LOG_S(INFO) << "Filter type match found in list for " << strres2 << "\nSet filterstyle key: " << j;

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
				LOG_S(INFO) << "Set filterrange: " << strres2;
			}

		}
		else
		{
			LOG_S(INFO) <<  "No filter/range statement found in string";
		}
	}

}

#ifdef TESTS_ENABLED
void utm_tests()
{
	LOG_S(INFO) << "UTM Tests begin";
	LOG_S(INFO) << "UTM Tests complete";
}
#endif
