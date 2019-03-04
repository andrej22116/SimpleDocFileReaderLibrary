#ifndef INPUTBINARYSTREAM_H
#define INPUTBINARYSTREAM_H

#include <iostream>

class InputBinaryStream : public std::istream
{
public:
    InputBinaryStream(std::streambuf* sb) : std::istream(sb) {}
    InputBinaryStream(std::istream& stream) : std::istream(stream.rdbuf()) {}

    template<typename T>
    inline T getData()
    {
        T result;
        this->read(reinterpret_cast<char*>(&result), sizeof(T));
        return result;
    }

    template<typename T>
    inline T getData(unsigned long long offset)
    {
        this->seekg(offset, std::ios_base::beg);
        T result = getData<T>();
        return result;
    }

    template<typename T>
    inline T peekData()
    {
        std::streampos oldOffset = this->tellg();
        T result = getData<T>(this->tellg());
        this->seekg(oldOffset, std::ios_base::beg);
        return result;
    }

    template<typename T>
    inline T peekData(unsigned long long offset)
    {
        std::streampos oldOffset = this->tellg();
        T result = getData<T>(offset);
        this->seekg(oldOffset, std::ios_base::beg);
        return result;
    }
};

#endif // INPUTBINARYSTREAM_H
