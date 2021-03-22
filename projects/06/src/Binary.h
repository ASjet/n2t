#ifndef BINARY_H
#define BINARY_H

////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
////////////////////////////////////////////////////////////////////////////////


class Binary
{
public:
    Binary() = default;
    Binary(char _Char) : _Int64(_Char) {_signed = true;}
    Binary(unsigned char _UnsignedChar) : _Int64(_UnsignedChar) {_signed = false;}
    Binary(short _Short) : _Int64(_Short) {_signed = true;}
    Binary(unsigned short _UnsignedShort) : _Int64(_UnsignedShort) {_signed = false;}
    Binary(int _Int) : _Int64(_Int) {_signed = true;}
    Binary(unsigned int _UnsignedInt) : _Int64(_UnsignedInt){_signed = false;}
    Binary(long _Long) : _Int64(_Long) {_signed = true;}
    Binary(unsigned long _UL) : _Int64(_UL) {_signed = false;}
    Binary(long long _LL) : _Int64(_LL) {_signed = true;}
    Binary(unsigned long long _ULL) : _Int64(_ULL) {_signed = false;}

    std::string str(const int _BitWidth, const unsigned int _SeperationSize) const;
    void set(const unsigned int target, const bool flag);
private:
    bool _signed;
    unsigned long long int _Int64;
};

#endif