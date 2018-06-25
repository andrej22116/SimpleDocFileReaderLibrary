#ifndef WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H
#define WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H

#include "stdincludes.h"
//#include "ClassStreamsMenager/streamsmenager.h"

#include "../ClassBinaryStreamWrapper/binarystreamwrapper.hpp"
#include "WindowsCompoundBinaryFileformat/wcbff_structures.h"
//#include "variablevisualize.hpp"

class WindowsCompoundBinaryFileFormatReader
{
private:
    WCBFF_FileHeader _header;
    uint32_t _sectorSize;
    uint32_t _miniSectorSize;

    std::vector<uint32_t> _difatChains;
    std::vector<uint32_t> _fatChains;
    std::vector<uint32_t> _miniFatChains;
    std::vector<WCBFF_DirectoryEntry> _directoryEntrys;
    std::vector<std::stringstream> _filesStreams;

    //StreamsMenager _streamsMenager;

public:
    WindowsCompoundBinaryFileFormatReader(std::istream& stream);

    static bool testOnWCBFF(uint64_t signature);

private:
    void readHeader(BinaryStreamWrapper& fBinStream);
    void readDIFChains(BinaryStreamWrapper& fBinStream);
    void readFATChains(BinaryStreamWrapper& fBinStream);
    void readMiniFATChains(BinaryStreamWrapper& fBinStream);
    void readDirectoryEntries(BinaryStreamWrapper& fBinStream);
    void readFilesStreams(BinaryStreamWrapper& fBinStream);
    void readDataToStream(std::istream& inputStream, std::ostream& outputStream,
                           uint32_t firstSector, uint32_t streamSize, bool isMiniFat);


//protected:
    uint32_t getStreamIdByName(std::string streamName);

};

#endif // WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H
