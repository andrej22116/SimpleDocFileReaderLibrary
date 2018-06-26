#pragma once

#ifndef VARIABLEVISUALIZE_HPP
#define VARIABLEVISUALIZE_HPP

#include "stdincludes.h"

#define WINDOWS_TICK 10000000
#define SEC_TO_UNIX_EPOCH 11644473600LL

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

template<typename T>
inline void showData(char* msg, T variable)
{
    std::cout << msg << variable << " - " << toHex(variable) << std::endl;
}

template<typename T>
inline void showDataInTableLine(char* msg, T variable)
{
    std::string message = msg;
    std::string decVariable = std::to_string(variable);
    std::string hexVariable = toHex(variable);
    char spacesForHex[] = "                       ";
    char spacesForDec[] = "                    ";

    spacesForHex[23 - hexVariable.size()] = '\0';
    hexVariable += spacesForHex;

    spacesForDec[20 - decVariable.size()] = '\0';
    decVariable += spacesForDec;

    if (message.size() > 34)
    {
        message.resize(31);
        message += "...";
    }

    std::cout << hexVariable << "|" << decVariable << "|" << message << std::endl;
}

inline std::string convert_UTF16_To_UTF8(std::u16string stringUTF16)
{
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>,char16_t> conversion;
    std::string res = conversion.to_bytes(stringUTF16);
    for (size_t i = 0; i < res.size(); i++)
    {
        if (res[i] < 0x20)
        {
            res.erase(res.begin() + i);
        }
        else
        {
            i++;
        }
    }
    return res;
}

/*
inline time_t fileTimeToUnix(WCBFF_FileTime& filetime) {
    long long ll_filetime = filetime.lowDateTime + (((long long)filetime.highDateTime) << 32);
    return (time_t)(ll_filetime / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
}

inline std::string convert_FileTime_To_UTF8(WCBFF_FileTime& fileTime)
{
    time_t thisTime = fileTimeToUnix(fileTime);
    return std::string(ctime(&thisTime));
}
*/

#endif // VARIABLEVISUALIZE_H
