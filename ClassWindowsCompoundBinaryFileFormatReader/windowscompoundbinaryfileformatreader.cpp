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
    for (auto shift : _difatChains)
    {
        uint32_t sectorOffset = (shift + 1) << _header.sectorShift;
        for (uint32_t i = sizeof(uint32_t); i < _sectorSize; i += sizeof(uint32_t))
        {
            uint32_t sector = fBinStream.getData<uint32_t>(sectorOffset + i);
            if (sector == WCBFF_EndOfChain)
            {
                this->_fatChains[chain[0] - 1] = chain;
                chain.clear();
            }
            else
            {
                chain.push_back(sector);
            }
        }
    }
}

void WindowsCompoundBinaryFileFormatReader::readMiniFATChains(BinaryStreamWrapper& fBinStream)
{
    std::vector<uint32_t> chain;
    uint32_t sectorOffset = (_header.miniFatFirstSectorAddres + 1) << _header.sectorShift;

    for (uint32_t i = 0; i < _header.miniFatSectorsAmount; i++)
    {
        uint32_t minisector = fBinStream.getData<uint32_t>(sectorOffset + i);
        chain.push_back(minisector);
    }
}









bool WindowsCompoundBinaryFileFormatReader::testOnWCBFF(uint64_t signature)
{
    return signature == WCBFF_ValidSignature || signature == WCBFF_ValidSignature_Old;
}
