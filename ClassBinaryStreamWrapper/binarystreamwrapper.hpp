#ifndef BINARYSTREAMWRAPPER_H
#define BINARYSTREAMWRAPPER_H

#include <fstream>

class BinaryStreamWrapper
{
private:
    std::ifstream* _stream;
    std::size_t _streamSize;

public:
    BinaryStreamWrapper(std::ifstream& stream);

    template<typename T>
    inline T getData()
    {
        T result;
        _stream->read(reinterpret_cast<char*>(&result), sizeof(T));
        return result;
    }

    template<typename T>
    inline T getData(unsigned long long offset)
    {
        _stream->seekg(offset, std::ios_base::beg);
        T result = getData<T>();
        return result;
    }

    template<typename T>
    inline T peekData()
    {
        std::streampos oldOffset = _stream->tellg();
        T result = getData<T>(_stream->tellg());
        _stream->seekg(oldOffset, std::ios_base::beg);
        return result;
    }

    template<typename T>
    inline T peekData(unsigned long long offset)
    {
        std::streampos oldOffset = _stream->tellg();
        T result = getData<T>(offset);
        _stream->seekg(oldOffset, std::ios_base::beg);
        return result;
    }

    inline std::size_t getStreamSize() { return _streamSize; }

private:
    void calculateStreamSize();
};

#endif // BINARYSTREAMWRAPPER_H
