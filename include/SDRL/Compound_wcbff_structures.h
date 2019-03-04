#pragma once

#ifndef CP_BFF_STRUCTURES_H
#define CP_WCBFF_STRUCTURES_H

#include <cinttypes>

enum WCBFF_ValidSignatures : uint64_t
{
    WCBFF_ValidSignature = 0xE11AB1A1E011CFD0,
    WCBFF_ValidSignature_Old = 0xE011CFD00DFC110E
};

enum WCBFF_SpecialValues : uint32_t
{
    WCBFF_DIFSector = 0xFFFFFFFC,
    WCBFF_FATSector = 0xFFFFFFFD,
    WCBFF_EndOfChain = 0xFFFFFFFE,
    WCBFF_ClearSector = 0xFFFFFFFF
};

enum WCBFF_STGTY : uint8_t
{
    STGTY_Invalid = 0,
    STGTY_Storage = 1,
    STGTY_Stream = 2,
    STGTY_Lockbytes = 3,
    STGTY_Property = 4,
    STGTY_Root = 5
};

enum WCBFF_DEColor : uint8_t
{
    DEColor_Red = 0,
    DEColor_Black = 1
};

// File date and time structure.
struct WCBFF_FileTime {
    uint32_t lowDateTime;
    uint32_t highDateTime;
};

struct WCBFF_FileHeader {
    uint64_t signature;

    uint8_t classID[16];

    uint16_t minorFormatVersion;
    uint16_t majorDllFormatVersion;

    uint16_t bytesOrdering;

    uint16_t sectorShift;
    uint16_t miniSectorShift;

    uint16_t reserved16_1;
    uint32_t reserver32_1;
    uint32_t reserver32_2;

    uint32_t fatSectorsAmount;
    uint32_t fatDirSectorAddres;

    uint32_t transactionSignature;

    uint32_t miniSectorCutoff;

    uint32_t miniFatChainsFirstSectorAddres;
    uint32_t miniFatSectorsAmount;

    uint32_t difFirstSectorAddres;
    uint32_t difSectorsAmount;

    uint32_t firstFatSectors[109];
};

struct WCBFF_DirectoryEntry {
    uint16_t elementName[32];   // In Unicode
    uint16_t elementNameLenght; // In characters

    WCBFF_STGTY objectType;
    WCBFF_DEColor colorOnTree;

    uint32_t leftSiblingSid;
    uint32_t rightSiblingSid;
    uint32_t childSid;

    uint8_t clsID[16];
    uint16_t userFlags;

    WCBFF_FileTime createTime;
    WCBFF_FileTime modifyTime;

    uint32_t sectorStartStream;
    uint32_t streamSize;

    uint16_t reserved;
};

#endif // WCBFF_STRUCTURES_H
