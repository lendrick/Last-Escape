#include "Utils.h"



/**
 * trim: remove leading and trailing c from s
 */
string trim(string s, char c) {
	if (s.length() == 0) return "";
	
	unsigned int first = 0;
	unsigned int last = s.length()-1;

	while (s.at(first) == c && first < s.length()-1) {
		first++;
	}
	while (s.at(last) == c && last >= first) {
		last--;
	}
	if (first <= last) return s.substr(first,last-first+1);
	return "";
}

/**
 * parse "[val]"
 */
string parse_section_title(string s) {
	unsigned int bracket = s.find_first_of(']');
	if (bracket == string::npos) return ""; // not found
	return s.substr(1, bracket-1);
}

/**
 * parse "key=val"
 */
void parse_key_pair(string s, string &key, string &val) {
	unsigned int separator = s.find_first_of('=');
	if (separator == string::npos) {
		key = "";
		val = "";
		return; // not found
	}
	key = s.substr(0, separator);
	val = s.substr(separator+1, s.length());
}
