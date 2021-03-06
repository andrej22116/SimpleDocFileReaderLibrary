#pragma once

#ifndef CP_BFFR_H
#define CP_BFFR_H

#include <SDRL/stdincludes.h>

namespace sdrl {

class WindowsCompoundBinaryFileFormatReader
{
public:
    WindowsCompoundBinaryFileFormatReader(std::istream& stream);
    WindowsCompoundBinaryFileFormatReader(const std::wstring& filename);

    uint32_t
    getStreamsAmount() { return _filesStreams.size(); }

    int32_t
    getStreamIdByName(std::string streamName);

    std::istream&
    getStreamById(uint32_t streamID);

    std::vector<std::pair<std::string, uint32_t>>
    getListOfStreams();

    void
    clearData();

    static bool
    testOnWCBFF(uint64_t signature);

private:
    struct _WCBFF_Structure;

    void
    read(_WCBFF_Structure& fileStructure);

    void
    readHeader(_WCBFF_Structure& fileStructure);

    void
    readDIFChains(_WCBFF_Structure& fileStructure);

    void
    readFATChains(_WCBFF_Structure& fileStructure);

    void
    readMiniFATChains(_WCBFF_Structure& fileStructure);

    void
    readDirectoryEntries(_WCBFF_Structure& fileStructure);

    void
    readFilesStreams(_WCBFF_Structure& fileStructure);

    void
    readDataToStream( _WCBFF_Structure& fileStructure
                    , std::istream& inputStream
                    , std::ostream& outputStream
                    , uint32_t firstSector
                    , uint32_t streamSize
                    , bool isMiniFat);

private:
    std::unordered_map<std::string, uint32_t> _filesStreamsNames;
    std::vector<std::stringstream> _filesStreams;
};

}

#endif // CP_BFFR_H
