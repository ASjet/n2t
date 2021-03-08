#include "utility.h"

void stringSplit(const std::string _String, const char _Char, std::vector<std::string>& _SubStrings)
{
    int start = 0, length = 0;
    _SubStrings.clear();
    for(auto i = _String.begin(); i != _String.end(); ++i)
    {
        if(*i == _Char)
        {
            if(length == 0)
                continue;
            else
            {
                _SubStrings.push_back(_String.substr(start,length));
                length = 0;
            }
        }
        else
        {
            if(length == 0)
                start = i - _String.begin();
            ++length;
        }
    }
    if(length != 0)
        _SubStrings.push_back(_String.substr(start,length));
}