#pragma once

#ifndef VARIABLEVISUALIZE_HPP
#define VARIABLEVISUALIZE_HPP

#include <sstream>

static inline void toCharLineToHex(std::string& res, unsigned char* begin, int len)
{
    char hexSymbols[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    unsigned char* posOnVariable = begin;
    std::stringstream result;

    for (int i = 0; i < len; i++)
    {
        unsigned char block = *posOnVariable;
        char data[] = { '0', '0', 0 };
        int offset = 1;

        while (block)
        {
            data[offset] = hexSymbols[block % 16];
            block /= 16;
            --offset;
        }
        result << data;
        result << '-';

        ++posOnVariable;
    }

    res = result.str();
    res.pop_back();
}

inline std::string toHex(const std::string string)
{
    std::string res;
    toCharLineToHex(res, (unsigned char*)(string.data()), string.length());
    return res;
}

template<typename T>
inline std::string toHex(T variable)
{
    std::string res;
    toCharLineToHex(res, reinterpret_cast<unsigned char*>(&variable), sizeof(T));
    return res;
}

#endif // VARIABLEVISUALIZE_H
