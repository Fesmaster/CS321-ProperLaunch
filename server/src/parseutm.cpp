#include "pch.hpp"
#include "parseutm.hpp"
#include <regex>



#define UTMERR(utmch, index) LOG_S(WARNING) << std::endl << "Error parsing utm" << std::endl << "Character: " << utmch << std::endl << "Index: " << index << std::endl

utmdata::utmdata(const std::string& utmstr)
: filterstyle(""), sortstyle(SortKey::DATE)
{
	if (!utmstr.empty()){
		LOG_S(INFO) << "CONSTRUCTOR: " << utmstr;
		std::regex sortsearch("[^\\s]+");

		auto words_begin = std::regex_iterator(utmstr.begin(), utmstr.end(), sortsearch);
		auto words_end = std::sregex_iterator();
		
		LOG_S(INFO) << "Found " 
              << std::distance(words_begin, words_end) 
              << " words:\n";

		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        	std::smatch match = *i;                                                 
        	std::string match_str = match.str(); 
        	LOG_S(INFO) << match_str << '\n';
    	}  

	}
}
