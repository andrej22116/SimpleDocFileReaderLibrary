#include "windowscompoundbinaryfileformatreader.h"

#include "variablevisualize.hpp"
#include "wcbff_structures.h"
#include "ClassInputBinaryStream/inputbinarystream.h"


struct WindowsCompoundBinaryFileFormatReader::_WCBFF_Structure {
    WCBFF_FileHeader header;
    uint32_t sectorSize;
    uint32_t miniSectorSize;

    std::vector<uint32_t> difatChains;
    std::vector<uint32_t> fatChains;
    std::vector<uint32_t> miniFatChains;
    std::vector<WCBFF_DirectoryEntry> directoryEntrys;

    InputBinaryStream& iStream;

    _WCBFF_Structure(InputBinaryStream& iStream) : iStream(iStream){}
};


WindowsCompoundBinaryFileFormatReader::WindowsCompoundBinaryFileFormatReader(std::istream& stream)
{
    //stream.rdbuf();
    InputBinaryStream binStream(stream.rdbuf());
    _WCBFF_Structure fileStructure(binStream);
    read(fileStructure);
}


WindowsCompoundBinaryFileFormatReader::WindowsCompoundBinaryFileFormatReader(const std::string& filename)
{
    std::ifstream file(filename, std::ios::binary);
    if (!file)
    {
        throw std::runtime_error("File " + filename + " not found!");
    }
    InputBinaryStream binStream(file.rdbuf());
    _WCBFF_Structure fileStructure(binStream);
    read(fileStructure);
}


void WindowsCompoundBinaryFileFormatReader::read(_WCBFF_Structure& fileStructure)
{
    readHeader(fileStructure);
    readDIFChains(fileStructure);
    readFATChains(fileStructure);
    readMiniFATChains(fileStructure);
    readDirectoryEntries(fileStructure);
}


void WindowsCompoundBinaryFileFormatReader::readHeader(_WCBFF_Structure& fileStructure)
{
    fileStructure.header = fileStructure.iStream.getData<WCBFF_FileHeader>();
    fileStructure.sectorSize = 1 << fileStructure.header.sectorShift;
    fileStructure.miniSectorSize = 1 << fileStructure.header.miniSectorShift;

    if (!testOnWCBFF(fileStructure.header.signature))
    {
        throw std::runtime_error("Invalid document header.");
    }
}


void WindowsCompoundBinaryFileFormatReader::readDIFChains(_WCBFF_Structure& fileStructure)
{
    fileStructure.difatChains.reserve(110);

    for (int i = 0; i < 109; i++)
    {
        fileStructure.difatChains.push_back(fileStructure.header.firstFatSectors[i]);
    }

    if (fileStructure.header.difFirstSectorAddres != WCBFF_EndOfChain)
    {
        uint32_t nextDiFatOffset = fileStructure.header.difFirstSectorAddres;

        for (uint32_t i = 0; i < fileStructure.header.difSectorsAmount && nextDiFatOffset != WCBFF_EndOfChain; i++)
        {
            uint32_t sectorBegin = (nextDiFatOffset + 1) << fileStructure.header.sectorShift;
            for (uint32_t j = 0, size = fileStructure.sectorSize - sizeof(uint32_t); j < size; j += sizeof(uint32_t))
            {
                fileStructure.difatChains.push_back(fileStructure.iStream.getData<uint32_t>(sectorBegin + j));
            }
            nextDiFatOffset = fileStructure.iStream.getData<uint32_t>(sectorBegin
                                                                         + fileStructure.sectorSize
                                                                         - sizeof(uint32_t));
        }
    }
}


void WindowsCompoundBinaryFileFormatReader::readFATChains(_WCBFF_Structure& fileStructure)
{
    for (auto shift : fileStructure.difatChains)
    {
        uint32_t sectorOffset = (shift + 1) << fileStructure.header.sectorShift;
        for (uint32_t i = 0; i < fileStructure.sectorSize; i += sizeof(uint32_t))
        {
            uint32_t sector = fileStructure.iStream.getData<uint32_t>(sectorOffset + i);
            fileStructure.fatChains.push_back(sector);
        }
    }
}


void WindowsCompoundBinaryFileFormatReader::readMiniFATChains(_WCBFF_Structure& fileStructure)
{
    uint32_t sectorOffset = (fileStructure.header.miniFatChainsFirstSectorAddres + 1) << fileStructure.header.sectorShift;

    for (uint32_t i = 0; i < fileStructure.header.miniFatSectorsAmount; i++)
    {
        for (uint32_t j = 0; j < fileStructure.sectorSize; j += sizeof(uint32_t))
        {
            uint32_t minisector = fileStructure.iStream.getData<uint32_t>(sectorOffset + j);
            fileStructure.miniFatChains.push_back(minisector);
        }
        if (sectorOffset < fileStructure.fatChains.size())
        {
            sectorOffset = fileStructure.fatChains[sectorOffset];
        }
        else
        {
            break;
        }
    }
}


void WindowsCompoundBinaryFileFormatReader::readDirectoryEntries(_WCBFF_Structure& fileStructure)
{
    uint32_t sector = fileStructure.header.fatDirSectorAddres;

    while( sector != WCBFF_EndOfChain )
    {
        uint32_t sectorOffset = (sector + 1) << fileStructure.header.sectorShift;
        for (uint32_t offsetInSector = 0;
             offsetInSector < fileStructure.sectorSize;
             offsetInSector += sizeof(WCBFF_DirectoryEntry))
        {
            auto dir = fileStructure.iStream.getData<WCBFF_DirectoryEntry>(sectorOffset + offsetInSector);
            if ( (static_cast<uint64_t>(fileStructure.iStream.gcount()) < sizeof(WCBFF_DirectoryEntry))
                 || (dir.objectType == STGTY_Invalid) )
            {
                break;
            }

            fileStructure.directoryEntrys.push_back(dir);
            std::string streamName = convert_UTF16_To_UTF8(std::u16string((char16_t*)dir.elementName));
            _filesStreamsNames[streamName] = fileStructure.directoryEntrys.size() - 1;

            /*
            std::cout << "Element name: " << streamName << std::endl;
            showDataInTableLine("Object type", dir.objectType);
            showDataInTableLine("Color on tree", dir.colorOnTree);
            showDataInTableLine("Left sibling sid", dir.leftSiblingSid);
            showDataInTableLine("Right sibling sid", dir.rightSiblingSid);
            showDataInTableLine("Child sid", dir.childSid);
            showDataInTableLine("Create time low", dir.createTime.lowDateTime);
            showDataInTableLine("Create time high", dir.createTime.highDateTime);
            showDataInTableLine("Modify time low", dir.modifyTime.lowDateTime);
            showDataInTableLine("Modify time high", dir.modifyTime.highDateTime);
            showDataInTableLine("Sector start stream", dir.sectorStartStream);
            showDataInTableLine("Stream size", dir.streamSize);
            std::cout << "Create time: " << convert_FileTime_To_UTF8(dir.createTime) << std::endl;
            std::cout << "Modify time: " << convert_FileTime_To_UTF8(dir.modifyTime) << std::endl;

            std::cout << std::endl;
            */
        }

        if (sector < fileStructure.fatChains.size())
        {
            sector = fileStructure.fatChains[sector];
        }
        else
        {
            break;
        }
    }

    _filesStreams.emplace_back();
    readDataToStream(fileStructure, fileStructure.iStream, _filesStreams[0],
            fileStructure.directoryEntrys[0].sectorStartStream,
            fileStructure.directoryEntrys[0].streamSize, false);
    readFilesStreams(fileStructure);
    /*
    int i = 0;
    for (auto& stream : _filesStreams)
        {
        BinaryStreamWrapper test(stream);
        std::cout << "Stream " << i << " size: " << test.getStreamSize() << std::endl;
        i++;
    }
    */
}


void WindowsCompoundBinaryFileFormatReader::readFilesStreams(_WCBFF_Structure& fileStructure)
{
    for(size_t streamID = 1; streamID < fileStructure.directoryEntrys.size(); streamID++)
    {
        _filesStreams.emplace_back();
        if (fileStructure.directoryEntrys[streamID].streamSize > 0)
        {
            if (fileStructure.directoryEntrys[streamID].streamSize < fileStructure.header.miniSectorCutoff)
            {
                readDataToStream(fileStructure, _filesStreams[0], _filesStreams[streamID],
                                 fileStructure.directoryEntrys[streamID].sectorStartStream,
                                 fileStructure.directoryEntrys[streamID].streamSize, true);
            }
            else
            {
                readDataToStream(fileStructure, fileStructure.iStream, _filesStreams[streamID],
                                 fileStructure.directoryEntrys[streamID].sectorStartStream,
                                 fileStructure.directoryEntrys[streamID].streamSize, false);
            }
        }
    }
}


void WindowsCompoundBinaryFileFormatReader::readDataToStream(_WCBFF_Structure& fileStructure,
                      std::istream& inputStream, std::ostream& outputStream,
                      uint32_t firstSector, uint32_t streamSize, bool isMiniFat)
{
    uint32_t sectorSize = isMiniFat ? fileStructure.miniSectorSize : fileStructure.sectorSize;
    uint32_t sectorShift = isMiniFat ? fileStructure.header.miniSectorShift : fileStructure.header.sectorShift;
    uint8_t headerOffset = isMiniFat ? 0 : 1;

    auto& chein = isMiniFat ? fileStructure.miniFatChains : fileStructure.fatChains;

    std::vector<char> buffer(sectorSize, 0);

    uint32_t sector = firstSector;
    uint32_t bytesNotReaded = streamSize;
    while (sector != WCBFF_EndOfChain)
    {
        uint32_t sectorOffset = (sector + headerOffset) << sectorShift;
        if (sector < chein.size())
        {
            sector = chein[sector];
        }
        else
        {
            break;
        }

        uint16_t bytesAmountForCopy = bytesNotReaded >= sectorSize ? sectorSize : bytesNotReaded;
        inputStream.seekg(sectorOffset, inputStream.beg);
        inputStream.read(buffer.data(), bytesAmountForCopy);
        outputStream.write(buffer.data(), bytesAmountForCopy);
        bytesNotReaded -= bytesAmountForCopy;
    }
}




int32_t WindowsCompoundBinaryFileFormatReader::getStreamIdByName(std::string streamName)
{
    auto iter = _filesStreamsNames.find(streamName);
    if (iter != _filesStreamsNames.end())
    {
        return iter->second;
    }
    else
    {
        return -1;
    }
}


std::istream& WindowsCompoundBinaryFileFormatReader::getStreamById(uint32_t streamID)
{
    if (streamID >= _filesStreams.size())
    {
        throw std::runtime_error("A stream with such an ID does not exist!");
    }
    return _filesStreams[streamID];
}


std::vector<std::pair<std::string, uint32_t>> WindowsCompoundBinaryFileFormatReader::getListOfStreams()
{
    std::vector<std::pair<std::string, uint32_t>> result;
    for (auto& streamNameAndId : _filesStreamsNames)
    {
        result.push_back(streamNameAndId);
    }
    return result;
}


bool WindowsCompoundBinaryFileFormatReader::testOnWCBFF(uint64_t signature)
{
    return signature == WCBFF_ValidSignature || signature == WCBFF_ValidSignature_Old;
}
