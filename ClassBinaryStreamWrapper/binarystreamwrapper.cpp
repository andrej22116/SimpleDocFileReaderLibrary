#include "ClassBinaryStreamWrapper/binarystreamwrapper.hpp"

BinaryStreamWrapper::BinaryStreamWrapper(std::istream& stream)
{
    _stream = &stream;
    calculateStreamSize();
}

void BinaryStreamWrapper::calculateStreamSize()
{
    std::streampos oldOffset = _stream->tellg();
    _stream->seekg(0, std::ios_base::end);
    _streamSize = _stream->tellg();
    _stream->seekg(oldOffset, std::ios_base::beg);
}

std::string BinaryStreamWrapper::getDataBlock(unsigned long long offset, size_t size)
{
    std::streampos oldOffset = _stream->tellg();
    _stream->seekg(offset, std::ios_base::end);

    std::string res;
    res.resize(size + 1);
    res[size] = 0;

    _stream->read((char*)res.data(), size);

    _stream->seekg(oldOffset, std::ios_base::beg);

    return res;
}
