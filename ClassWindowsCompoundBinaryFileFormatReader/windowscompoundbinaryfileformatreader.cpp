#include "windowscompoundbinaryfileformatreader.h"
#include <iostream>
#include <string>
#include <algorithm>
#include "variablevisualize.hpp"

WindowsCompoundBinaryFileFormatReader::WindowsCompoundBinaryFileFormatReader(std::istream& stream)
    : _memoryStream(&_streamBuffer)
{
    auto oldStreamOffset = stream.tellg();
    stream.seekg(0, stream.beg);
    _memoryStream << stream.rdbuf();
    stream.seekg(oldStreamOffset, stream.beg);

    _memoryStream.seekg(0, _memoryStream.beg);
    BinaryStreamWrapper fBinStream(_memoryStream);
    //std::cout << "Need long number: " << toHex(validHeaderBegin) << std::endl;
    if (!testOnCFB(fBinStream.getData<unsigned long long>()))
    {
        throw std::runtime_error("Invalid document header.");
    }

    readHeader(fBinStream);
    readDIFATChains(fBinStream);
    readFATChains(fBinStream);
}


void WindowsCompoundBinaryFileFormatReader::readHeader(BinaryStreamWrapper& fBinStream)
{
    std::cout << "\n[_._._._HEADER_._._._]" << std::endl;

    // Get file system version.
    this->_version = fBinStream.getData<unsigned short>(0x1A);
    std::cout << "[HEADER] File system version: " << toHex(this->_version) << std::endl;

    // Get order bytes in file.
    auto bytesOrder = fBinStream.getData<unsigned short>(0x1C);
    std::cout << "[HEADER] Order bytes: " << toHex(bytesOrder) << std::endl;
    _isLittleEndian = bytesOrder == 0xFFFE;

    // Get sectors offsets.
    this->_sectorShift_FAT = fBinStream.getData<unsigned short>(0x1E);
    this->_sectorShift_MiniFAT = fBinStream.getData<unsigned short>(0x20);
    this->_maxStreamSize_miniFAT_Bits = fBinStream.getData<unsigned int>(0x38);

    std::cout << "[HEADER] FAT sector shift: " << toHex(this->_sectorShift_FAT) << std::endl;
    std::cout << "[HEADER] Mini FAT sector shift: " << toHex(this->_sectorShift_MiniFAT) << std::endl;
    std::cout << "[HEADER] Max mini FAT stream size: " << toHex(this->_maxStreamSize_miniFAT_Bits) << std::endl;

    if (this->_version == 0x04)
    {
        // Files amount.
        this->_filesAmount = fBinStream.getData<unsigned int>(0x28);
        std::cout << "[HEADER] Files amount: " << toHex(this->_filesAmount) << std::endl;
    }
    // Offset to first file.
    this->_firstFileOffset = fBinStream.getData<unsigned int>(0x30);
    std::cout << "[HEADER] First file offset: " << toHex(this->_firstFileOffset) << std::endl;

    // FAT sectors amount.
    this->_sectorsAmount_FAT = fBinStream.getData<unsigned int>(0x2C);
    std::cout << "[HEADER] FAT sectors amount: " << toHex(this->_sectorsAmount_FAT) << std::endl;

    // First mini FAT sector location.
    this->_firstSectorsOffset_MiniFAT = fBinStream.getData<unsigned int>(0x3C);
    std::cout << "[HEADER] Mini FAT first sector offset: " << toHex(this->_firstSectorsOffset_MiniFAT) << std::endl;
    // Mini FAT sectors amount.
    this->_sectorsAmount_MiniFAT = fBinStream.getData<unsigned int>(0x40);
    std::cout << "[HEADER] Mini FAT sectors amount: " << toHex(this->_sectorsAmount_MiniFAT) << std::endl;

    // First DIFAT sector location.
    this->_firstSectorsOffset_DIFAT = fBinStream.getData<unsigned int>(0x44);
    this->_sectorsAmount_DIFAT = fBinStream.getData<unsigned int>(0x48);
    std::cout << "[HEADER] DIFAT first sector offset: " << toHex(this->_firstSectorsOffset_DIFAT) << std::endl;
    std::cout << "[HEADER] DIFAT sectors amount: " << toHex(this->_sectorsAmount_DIFAT) << std::endl;
}

void WindowsCompoundBinaryFileFormatReader::readDIFATChains(BinaryStreamWrapper& fBinStream)
{
    std::cout << "\n[_._._._DIFAT_CHAINS_._._._]" << std::endl;
    _difatChains.reserve(110);

    std::cout << "[DIFAT_CHAINS] Read first 109 links to chains." << std::endl;
    for (int i = 0; i < 109; i++)
    {
        _difatChains.push_back(fBinStream.getData<unsigned int>(0x4C + i * sizeof(unsigned int)));
    }

    if (this->_firstSectorsOffset_DIFAT != endOfChain)
    {
        std::cout << "[DIFAT_CHAINS] Read other links to chains(File size > 8.5MB)." << std::endl;
        unsigned int sectorSize = 1 << this->_sectorShift_FAT;
        unsigned int nextDiFatOffset = this->_firstSectorsOffset_DIFAT;

        for (unsigned int i = 0; i < this->_sectorsAmount_DIFAT && nextDiFatOffset != endOfChain; i++)
        {
            unsigned int sectorBegin = (nextDiFatOffset + 1) << this->_sectorShift_FAT;
            for (unsigned int j = 0, size = sectorSize - sizeof(unsigned int); j < size; j += sizeof(unsigned int))
            {
                _difatChains.push_back(fBinStream.getData<unsigned int>(sectorBegin + j));
            }
            nextDiFatOffset = fBinStream.getData<unsigned int>(sectorBegin + sectorSize - sizeof(unsigned int));
        }

        _difatChains.erase(
            std::remove_if(_difatChains.begin(), _difatChains.end(), [](unsigned int addres) {
                return addres == clearSector;
            }),
            _difatChains.end()
        );
    }
}

void WindowsCompoundBinaryFileFormatReader::readFATChains(BinaryStreamWrapper& fBinStream)
{
     //std::cout << "\n[_._._._FAT_CHAINS_._._._]" << std::endl;
     unsigned int sectorSize = 1 << this->_sectorShift_FAT;
     this->_fatChains.reserve(sectorSize / sizeof(unsigned int) * _difatChains.size());

     for (auto shift : _difatChains)
     {
         unsigned int sectorOffset = (shift + 1) << this->_sectorShift_FAT;
         for (unsigned int i = 0; i < sectorSize; i += sizeof(int))
         {
              this->_fatChains.push_back(fBinStream.getData<unsigned int>(sectorOffset + i));
         }
     }
}









bool WindowsCompoundBinaryFileFormatReader::testOnCFB(unsigned long long firstBytesFromFile)
{
    return firstBytesFromFile == validHeaderBegin || firstBytesFromFile == validOldHeaderBegin;
}
