#include "Binary.h"
////////////////////////////////////////////////////////////////////////////////
std::string Binary::str(const int _BitWidth, const unsigned int _SeperationSize) const
{

    int count = 0;
    unsigned long long flag = 1ULL << (_BitWidth - 1);
    std::ostringstream bin_str;
    if (_BitWidth % _SeperationSize)
    {
        bin_str << std::string(_SeperationSize - (_BitWidth % _SeperationSize), '0');
        for (int i = 0; i != (_BitWidth % _SeperationSize); ++i, flag >>= 1)
            bin_str << (flag & _Int64);
    }

    for (int i = 0; flag != 0; ++i, flag >>= 1)
    {
        if ((i % _SeperationSize) == 0 && i != 0)
            bin_str << ' ';
        bin_str << bool(flag & _Int64);
    }
    return bin_str.str();
}
////////////////////////////////////////////////////////////////////////////////
void Binary::set(const unsigned int target, const bool flag)
{
    unsigned long long p = 1ULL << target;
    (flag) ? _Int64 &= p : _Int64 |= ~p;
}