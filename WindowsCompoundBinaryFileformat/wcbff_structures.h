#ifndef WCBFF_STRUCTURES_H
#define WCBFF_STRUCTURES_H

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

struct FIB_Base {
    uint16_t fileIdentifier;    // For Word document should be 0xA5EC
    uint16_t fileFormatVersion; // Should be 0x00C1

    uint16_t unused;

    uint16_t languageID;
    uint16_t pnNext; // ???

    unsigned A_fDot : 1;                    // A - Document is Template, if set 1.
    unsigned B_fGlsy : 1;                   // B - Document has only aoutotext, if set 1.
    unsigned C_fComplex : 1;                // C - Huy znaett, last save-operation is incremental.
    unsigned D_fHasPic : 1;                 // D - Document has pictures, if set 1.
    unsigned E_cQuickSaves : 4;             // E - If fileFormatVersion < 0x00D9 - amount of saves, else 0xF.
    unsigned F_fEncrypted : 1;              // F - File is encrypted, if set 1.
    unsigned G_fWhichTblStm : 1;            // G - Stream table id. If set 1 then use 1Table, else - 0Table.
    unsigned H_fReadOnlyRecommended : 1;    // H - Author recomended read only, if set 1.
    unsigned I_fWriteReservation : 1;       // I - has password for modify document, if set 1.
    unsigned J_fExtChar : 1;                // J - Should be set 1. I don't know, why is so.
    unsigned K_fLoadOverride : 1;           // K - Need override information of language and font, if set 1.
    unsigned L_fFarEast : 1;                // L - Было ли приложение, создавшее документ - азиатским... СУКА, АЗИАТСКИМ, КАКОГО ХРЕНА?
    unsigned M_fObfuscated : 1;             // M - If F_fEncrypted = 1 - use XOR obfuscation, if set 1.

    uint16_t nFidBack : 16;     // Что-то про версию документа.
    uint16_t lKey[2];           // I hate Microsoft! This array - uint32_t variable.

    uint8_t envr;           // Ignore this

    unsigned N_fMac : 1;                // N - Ignore this, should be 0.
    unsigned O_fEmptySpecial : 1;       // O - Скорее всего будет 0.
    unsigned P_fLoadOverridePage : 1;   // P - Need override page properties, if set 1.
    unsigned Q_reserved_1 : 1;
    unsigned R_reserved_2 : 1;
    unsigned S_reserved_3 : 3;

    uint16_t reserved3;
    uint16_t reserved4;
    uint32_t reserved5;
    uint32_t reserved6;

};

#define LKEY_TO_UINT32(fib_base_struct) \
    uint32_t((fib_base_struct.lKey[1] << 16) + (fib_base_struct.lKey[0]))

#define UINT32_TO_LKEY(fib_base_struct, value) \
    *((uint32_t*)fib_base_struct.lKey) = value

#endif // WCBFF_STRUCTURES_H
