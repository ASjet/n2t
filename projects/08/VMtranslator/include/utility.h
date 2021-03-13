#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>
#include <cctype>

////////////////////////////////////////////////////////////////////////////////
void stringSplit(const std::string _String, const char _Char, std::vector<std::string>& _SubStrings);
bool isComment(const std::string _CommandLine);
#endif
