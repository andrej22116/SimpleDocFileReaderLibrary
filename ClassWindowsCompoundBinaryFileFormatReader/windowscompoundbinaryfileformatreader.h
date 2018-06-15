#ifndef WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H
#define WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H


#include <istream>
#include <sstream>
#include <string>
#include <vector>

#include "../ClassBinaryStreamWrapper/binarystreamwrapper.hpp"


////////// Constants
// End of data chain in sectors.
const unsigned int endOfChain = 0xFFFFFFEF;

// End of data chain in sectors.
const unsigned int clearSector = 0xFFFFFFFF;

// Sectors sizes
const unsigned short sectorSize_FAT_Bytes = 64;
const unsigned short sectorSize_FAT_Bits = 512;

const unsigned short sectorSize_miniFat_Bytes = 8;
const unsigned short sectorSize_miniFat_Bits = 64;

// References header begins.
const unsigned long long validHeaderBegin = 0xE11AB1A1E011CFD0;
const unsigned long long validOldHeaderBegin = 0xE011CFD00DFC110E;
////////// ...



class WindowsCompoundBinaryFileFormatReader
{
private:
    long long _dateOfCreate;

    // FAT
    unsigned short _sectorShift_FAT = 0;
    unsigned int _sectorsAmount_FAT = 0;
    unsigned int _firstSectorsOffset_FAT = 0;
    std::vector<unsigned int> _fatChains;
    std::vector<unsigned int> _fatEntries;

    // Mini FAT
    unsigned short _sectorShift_MiniFAT = 0;
    unsigned int _sectorsAmount_MiniFAT = 0;
    unsigned int _firstSectorsOffset_MiniFAT = 0;
    unsigned int _maxStreamSize_miniFAT_Bits = 0;
    std::vector<unsigned int> _miniFatChains;
    std::string _miniFAT;       //????

    // DIFAT
    unsigned int _sectorsAmount_DIFAT = 0;
    unsigned int _firstSectorsOffset_DIFAT = 0;
    std::vector<unsigned int> _difatChains;

    // Document structure information.
    unsigned char _version = 3;
    bool _isLittleEndian = true;

    // Files offset.
    unsigned int _filesAmount = 0;
    unsigned int _firstFileOffset = 0;

    // Memory
    std::stringbuf _streamBuffer;
    std::iostream _memoryStream;
public:
    WindowsCompoundBinaryFileFormatReader(std::istream& stream);

    static bool testOnCFB(unsigned long long firstBytesFromFile);

private:
    void readHeader(BinaryStreamWrapper& fBinStream);
    void readDIFATChains(BinaryStreamWrapper& fBinStream);
    void readFATChains(BinaryStreamWrapper& fBinStream);
};

#endif // WINDOWSCOMPOUNDBINARYFILEFORMATREADER_H
