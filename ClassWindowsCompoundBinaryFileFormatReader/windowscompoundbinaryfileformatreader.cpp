#include "windowscompoundbinaryfileformatreader.h"
#include <iostream>
#include <string>
#include <algorithm>

#include "variablevisualize.hpp"

WindowsCompoundBinaryFileFormatReader::WindowsCompoundBinaryFileFormatReader(std::istream& stream)
{
    BinaryStreamWrapper fBinStream(stream);
    readHeader(fBinStream);
    readDIFChains(fBinStream);
    readFATChains(fBinStream);
    readMiniFATChains(fBinStream);
    createFilesStreams(fBinStream);
    std::string str = dynamic_cast<std::stringstream&>(_streamsMenager.getStream("WordDocument")).str();
    str = convert_UTF16_To_UTF8(std::u16string((char16_t*)str.data()));
    std::cout << str << std::endl;
}


void WindowsCompoundBinaryFileFormatReader::readHeader(BinaryStreamWrapper& fBinStream)
{
    //std::cout << "\n[_._._._HEADER_._._._]" << std::endl;
    _header = fBinStream.getData<WCBFF_FileHeader>();
    _sectorSize = 1 << _header.sectorShift;

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

    _difatChains.erase(
        std::remove_if(_difatChains.begin(), _difatChains.end(), [](unsigned int addres) {
            return addres == WCBFF_ClearSector;
        }),
        _difatChains.end()
    );
}

void WindowsCompoundBinaryFileFormatReader::readFATChains(BinaryStreamWrapper& fBinStream)
{
    std::vector<uint32_t> chain;
    std::vector<uint32_t> test;
    for (auto shift : _difatChains)
    {
        uint32_t sectorOffset = (shift + 1) << _header.sectorShift;
        for (uint32_t i = 0; i < _sectorSize; i += sizeof(uint32_t))
        {
            uint32_t sector = fBinStream.getData<uint32_t>(sectorOffset + i);
            test.push_back(sector);
            if (sector == WCBFF_EndOfChain && chain.size() > 0)
            {
                this->_fatChains[chain[0] - 1] = chain;
                chain.clear();
            }
            else if (sector >= 0xFFFFFFFA)
            {
                continue;
            }
            else
            {
                chain.push_back(sector);
            }
        }
    }
    int i = 0;
    i++;
}

void WindowsCompoundBinaryFileFormatReader::readMiniFATChains(BinaryStreamWrapper& fBinStream)
{
    std::vector<uint32_t> chain;
    uint32_t sectorOffset = (_header.miniFatFirstSectorAddres + 1) << _header.sectorShift;

    for (uint32_t i = 0; i < _header.miniFatSectorsAmount; i++)
    {
        for (uint32_t j = 0; j < _sectorSize; j += sizeof(uint32_t))
        {
            uint32_t minisector = fBinStream.getData<uint32_t>(sectorOffset + j);
            if (minisector == WCBFF_EndOfChain && chain.size() > 0)
            {
                this->_miniFatChains[chain[0] - 1] = chain;
                chain.clear();
            }
            else if (minisector >= 0xFFFFFFFA)
            {
                continue;
            }
            else
            {
                chain.push_back(minisector);
            }
        }
    }
}

void WindowsCompoundBinaryFileFormatReader::createFilesStreams(BinaryStreamWrapper& fBinStream)
{
    std::cout << "\n///////////////////// TEST //////////////////////" << std::endl;
    uint32_t sector = _fatChains[_header.fatDirSectorAddres][0];
    std::set<uint32_t> used;
    std::queue<uint32_t> entryQueue;
    entryQueue.push(0);

    uint32_t offset = sector << _header.sectorShift;
    while(!entryQueue.empty())
    {
        uint32_t listNumber = entryQueue.front();
        entryQueue.pop();

        if (used.find(listNumber) != used.end())
        {
            continue;
        }
        used.insert(listNumber);

        auto dir = fBinStream.getData<WCBFF_DirectoryEntry>(offset + sizeof(WCBFF_DirectoryEntry) * listNumber);
        std::string streamName = convert_UTF16_To_UTF8(std::u16string((char16_t*)dir.elementName));

        if (dir.leftSiblingSid  != WCBFF_ClearSector) { entryQueue.push(dir.leftSiblingSid); }
        if (dir.rightSiblingSid != WCBFF_ClearSector) { entryQueue.push(dir.rightSiblingSid); }
        if (dir.childSid        != WCBFF_ClearSector) { entryQueue.push(dir.childSid); }

        if (dir.objectType == STGTY_Stream)
        {
            _streamsMenager.createStream(listNumber, streamName);
            auto& stream = _streamsMenager.getStream(listNumber);
            for (uint32_t streamSector : _fatChains[dir.sectorStartStream])
            {
                uint32_t streamOffset = streamSector << _header.sectorShift;
                for (uint32_t offsetForRead = 0; offsetForRead < _sectorSize; offsetForRead += sizeof(uint64_t))
                {
                    uint64_t data = fBinStream.getData<uint64_t>(streamOffset + offsetForRead);
                    stream.write((char*)&data, sizeof(data));
                }
            }
            stream.seekg(0, stream.beg);
        }

        std::cout << "Element name: " << streamName << std::endl;
        showDataInTableLine("Struct size", sizeof(WCBFF_DirectoryEntry));
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
}







bool WindowsCompoundBinaryFileFormatReader::testOnWCBFF(uint64_t signature)
{
    return signature == WCBFF_ValidSignature || signature == WCBFF_ValidSignature_Old;
}
