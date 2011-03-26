#ifndef UTILS_H
#define UTILS_H

#include <string>

using namespace std;

string trim(string s, char c);
string parse_section_title(string s);
void parse_key_pair(string s, string &key, string &val);

#endif;
