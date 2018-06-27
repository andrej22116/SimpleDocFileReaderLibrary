#pragma once

#ifndef WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H
#define WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H

#include "stdincludes.h"

class WindowsCompoundBinaryFileFormatReader
{
private:
    struct _WCBFF_Structure;

    std::map<std::string, uint32_t> _filesStreamsNames;
    std::vector<std::stringstream> _filesStreams;

public:
    WindowsCompoundBinaryFileFormatReader(std::istream& stream);
    WindowsCompoundBinaryFileFormatReader(const std::string& filename);

    inline uint32_t getStreamsAmount() { return _filesStreams.size(); }
    int32_t getStreamIdByName(std::string streamName);
    std::istream& getStreamById(uint32_t streamID);
    std::vector<std::pair<std::string, uint32_t>> getListOfStreams();


    static bool testOnWCBFF(uint64_t signature);

private:
    void read(_WCBFF_Structure& fileStructure);
    void readHeader(_WCBFF_Structure& fileStructure);
    void readDIFChains(_WCBFF_Structure& fileStructure);
    void readFATChains(_WCBFF_Structure& fileStructure);
    void readMiniFATChains(_WCBFF_Structure& fileStructure);
    void readDirectoryEntries(_WCBFF_Structure& fileStructure);
    void readFilesStreams(_WCBFF_Structure& fileStructure);
    void readDataToStream(_WCBFF_Structure& fileStructure,
                          std::istream& inputStream, std::ostream& outputStream,
                          uint32_t firstSector, uint32_t streamSize, bool isMiniFat);
};

#endif // WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H
