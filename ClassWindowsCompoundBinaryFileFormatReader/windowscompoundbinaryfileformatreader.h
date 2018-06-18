#ifndef WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H
#define WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H

#include "stdincludes.h"
#include "ClassStreamsMenager/streamsmenager.h"

#include "../ClassBinaryStreamWrapper/binarystreamwrapper.hpp"
#include "WindowsCompoundBinaryFileformat/wcbff_structures.h"
//#include "variablevisualize.hpp"

class WindowsCompoundBinaryFileFormatReader
{
private:
    WCBFF_FileHeader _header;
    uint32_t _sectorSize;

    std::vector<uint32_t> _difatChains;
    std::map<uint32_t, std::vector<uint32_t>> _fatChains;
    std::map<uint32_t, std::vector<uint32_t>> _miniFatChains;

    StreamsMenager _streamsMenager;

public:
    WindowsCompoundBinaryFileFormatReader(std::istream& stream);

    static bool testOnWCBFF(uint64_t signature);

private:
    void readHeader(BinaryStreamWrapper& fBinStream);
    void readDIFChains(BinaryStreamWrapper& fBinStream);
    void readFATChains(BinaryStreamWrapper& fBinStream);
    void readMiniFATChains(BinaryStreamWrapper& fBinStream);
    void createFilesStreams(BinaryStreamWrapper& fBinStream);
};

#endif // WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H
