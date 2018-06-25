#include "windowscompoundbinaryfileformatreader.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>

#include "variablevisualize.hpp"

WindowsCompoundBinaryFileFormatReader::WindowsCompoundBinaryFileFormatReader(std::istream& stream)
{
    BinaryStreamWrapper fBinStream(stream);
    readHeader(fBinStream);
    readDIFChains(fBinStream);
    readFATChains(fBinStream);
    readMiniFATChains(fBinStream);
    readDirectoryEntries(fBinStream);

    //std::string str = dynamic_cast<std::stringstream&>(_streamsMenager.getStream("WordDocument")).str();
    //str = convert_UTF16_To_UTF8(std::u16string((char16_t*)str.data()));
    //std::cout << str << std::endl;
}


void WindowsCompoundBinaryFileFormatReader::readHeader(BinaryStreamWrapper& fBinStream)
{
    //std::cout << "\n[_._._._HEADER_._._._]" << std::endl;
    _header = fBinStream.getData<WCBFF_FileHeader>();
    _sectorSize = 1 << _header.sectorShift;
    _miniSectorSize = 1 << _header.miniSectorShift;

    if (!testOnWCBFF(_header.signature))
    {
        throw std::runtime_error("Invalid document header.");
    }
}

void WindowsCompoundBinaryFileFormatReader::readDIFChains(BinaryStreamWrapper& fBinStream)
{
    //std::cout << "\n[_._._._DIFAT_CHAINS_._._._]" << std::endl;
    _difatChains.reserve(110);

    for (int i = 0; i < 109; i++)
    {
        _difatChains.push_back(_header.firstFatSectors[i]);
    }

    if (_header.difFirstSectorAddres != WCBFF_EndOfChain)
    {
        uint32_t nextDiFatOffset = _header.difFirstSectorAddres;

        for (uint32_t i = 0; i < _header.difSectorsAmount && nextDiFatOffset != WCBFF_EndOfChain; i++)
        {
            uint32_t sectorBegin = (nextDiFatOffset + 1) << _header.sectorShift;
            for (uint32_t j = 0, size = _sectorSize - sizeof(uint32_t); j < size; j += sizeof(uint32_t))
            {
                _difatChains.push_back(fBinStream.getData<uint32_t>(sectorBegin + j));
            }
            nextDiFatOffset = fBinStream.getData<uint32_t>(sectorBegin + _sectorSize - sizeof(uint32_t));
        }
    }
}

void WindowsCompoundBinaryFileFormatReader::readFATChains(BinaryStreamWrapper& fBinStream)
{
    for (auto shift : _difatChains)
    {
        uint32_t sectorOffset = (shift + 1) << _header.sectorShift;
        for (uint32_t i = 0; i < _sectorSize; i += sizeof(uint32_t))
        {
            uint32_t sector = fBinStream.getData<uint32_t>(sectorOffset + i);
            _fatChains.push_back(sector);
        }
    }
}

void WindowsCompoundBinaryFileFormatReader::readMiniFATChains(BinaryStreamWrapper& fBinStream)
{
    uint32_t sectorOffset = (_header.miniFatChainsFirstSectorAddres + 1) << _header.sectorShift;

    for (uint32_t i = 0; i < _header.miniFatSectorsAmount; i++)
    {
        for (uint32_t j = 0; j < _sectorSize; j += sizeof(uint32_t))
        {
            uint32_t minisector = fBinStream.getData<uint32_t>(sectorOffset + j);
            _miniFatChains.push_back(minisector);
        }
        if (sectorOffset < _fatChains.size())
        {
            sectorOffset = _fatChains[sectorOffset];
        }
        else
        {
            break;
        }
    }
}

void WindowsCompoundBinaryFileFormatReader::readDirectoryEntries(BinaryStreamWrapper& fBinStream)
{
    std::cout << "\n///////////////////// TEST //////////////////////" << std::endl;

    uint32_t idForStreams = 0;
    uint32_t sector = _header.fatDirSectorAddres;

    while( sector != WCBFF_EndOfChain )
    {
        uint32_t sectorOffset = (sector + 1) << _header.sectorShift;
        for (int offsetInSector = 0; offsetInSector < _sectorSize; offsetInSector += sizeof(WCBFF_DirectoryEntry))
        {
            auto dir = fBinStream.getData<WCBFF_DirectoryEntry>(sectorOffset + offsetInSector);
            if ( (fBinStream.getBytesReadedCount() < sizeof(WCBFF_DirectoryEntry))
                 || (dir.objectType == STGTY_Invalid) )
            {
                break;
            }

            _directoryEntrys.push_back(dir);

            std::string streamName = convert_UTF16_To_UTF8(std::u16string((char16_t*)dir.elementName));
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
        }

        if (sector < _fatChains.size())
        {
            sector = _fatChains[sector];
        }
        else
        {
            break;
        }
    }

    _filesStreams.emplace_back();
    auto& fStream = fBinStream.getStream();
    readDataToStream(fStream, _filesStreams[0],
            _directoryEntrys[0].sectorStartStream,
            _directoryEntrys[0].streamSize, false);
    readFilesStreams(fBinStream);

    int i = 0;
    for (auto& stream : _filesStreams)
        {
        BinaryStreamWrapper test(stream);
        std::cout << "Stream " << i << " size: " << test.getStreamSize() << std::endl;
        i++;
    }
}

void WindowsCompoundBinaryFileFormatReader::readFilesStreams(BinaryStreamWrapper& fBinStream)
{
    auto& fStream = fBinStream.getStream();

    for(int streamID = 1; streamID < _directoryEntrys.size(); streamID++)
    {
        _filesStreams.emplace_back();
        if (_directoryEntrys[streamID].streamSize > 0)
        {
            if (_directoryEntrys[streamID].streamSize < _header.miniSectorCutoff)
            {
                readDataToStream(_filesStreams[0], _filesStreams[streamID],
                                 _directoryEntrys[streamID].sectorStartStream,
                                 _directoryEntrys[streamID].streamSize, true);
            }
            else
            {
                readDataToStream(fStream, _filesStreams[streamID],
                                 _directoryEntrys[streamID].sectorStartStream,
                                 _directoryEntrys[streamID].streamSize, false);
            }
        }
    }
}

void WindowsCompoundBinaryFileFormatReader::readDataToStream(std::istream& inputStream, std::ostream& outputStream,
                       uint32_t firstSector, uint32_t streamSize, bool isMiniFat)
{
    uint32_t sectorSize = isMiniFat ? _miniSectorSize : _sectorSize;
    uint32_t sectorShift = isMiniFat ? _header.miniSectorShift : _header.sectorShift;
    uint8_t headerOffset = isMiniFat ? 0 : 1;

    auto& chein = isMiniFat ? _miniFatChains : _fatChains;

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



bool WindowsCompoundBinaryFileFormatReader::testOnWCBFF(uint64_t signature)
{
    return signature == WCBFF_ValidSignature || signature == WCBFF_ValidSignature_Old;
}
