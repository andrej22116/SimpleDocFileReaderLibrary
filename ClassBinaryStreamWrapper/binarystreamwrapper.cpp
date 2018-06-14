#include "ClassBinaryStreamWrapper/binarystreamwrapper.hpp"

BinaryStreamWrapper::BinaryStreamWrapper(std::ifstream& stream)
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
