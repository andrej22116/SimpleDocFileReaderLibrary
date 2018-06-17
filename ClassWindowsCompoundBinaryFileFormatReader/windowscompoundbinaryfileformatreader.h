#ifndef WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H
#define WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H


#include <istream>
#include <sstream>
#include <string>
#include <vector>
#include <map>

#include "../ClassBinaryStreamWrapper/binarystreamwrapper.hpp"
#include "WindowsCompoundBinaryFileformat/wcbff_structures.h"
//#include "variablevisualize.hpp"

class WindowsCompoundBinaryFileFormatReader
{
private:
    WCBFF_FileHeader _header;
    uint32_t _sectorSize;

    std::map<uint32_t, std::vector<uint32_t>> _fatChains;
    std::map<uint32_t, std::vector<uint32_t>> _miniFatChains;
    //std::vector<uint32_t> _fatEntries;

    //std::string _miniFAT;

    std::vector<uint32_t> _difatChains;

    // Memory
    //std::stringbuf _streamBuffer;
    //std::iostream _memoryStream;
public:
    WindowsCompoundBinaryFileFormatReader(std::istream& stream);

    static bool testOnWCBFF(uint64_t signature);

private:
    void readHeader(BinaryStreamWrapper& fBinStream);
    void readDIFChains(BinaryStreamWrapper& fBinStream);
    void readFATChains(BinaryStreamWrapper& fBinStream);
    void readMiniFATChains(BinaryStreamWrapper& fBinStream);
};

#endif // WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H
