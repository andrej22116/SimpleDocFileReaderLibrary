#ifndef WBFF_STRUCTURES_H
#define WBFF_STRUCTURES_H

#include <cinttypes>

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

struct FIB_RgW97
{
    uint16_t reserver_1;
    uint16_t reserver_2;
    uint16_t reserver_3;
    uint16_t reserver_4;
    uint16_t reserver_5;
    uint16_t reserver_6;
    uint16_t reserver_7;
    uint16_t reserver_8;
    uint16_t reserver_9;
    uint16_t reserver_10;
    uint16_t reserver_11;
    uint16_t reserver_12;
    uint16_t reserver_13;

    uint16_t lidFE;
};

struct FIB_RgLw97
{
    uint32_t cbMac;

    int32_t reserver_1;
    int32_t reserver_2;

    int32_t ccpText;
    int32_t ccpFtn;
    int32_t ccpHdd;

    int32_t reserver_3;

    int32_t ccpAtn;
    int32_t ccpEdn;
    int32_t ccpTxbx;
    int32_t ccpHdrTxbx;

    int32_t reserver_4;
    int32_t reserver_5;
    int32_t reserver_6;
    int32_t reserver_7;
    int32_t reserver_8;
    int32_t reserver_9;
    int32_t reserver_10;
    int32_t reserver_11;
    int32_t reserver_12;
    int32_t reserver_13;
    int32_t reserver_14;
};

struct FIB_RgFcLcb97
{
    uint32_t ignore_fcStshfOrig;
    uint32_t ignore_lcbStshfOrig;

    uint32_t fcStshf;
    uint32_t lcbStshf;

    uint32_t fcPlcffndRef;
    uint32_t lcbPlcffndRef;

    uint32_t fcPlcffndTxt;
    uint32_t lcbPlcffndTxt;

    uint32_t fcPlcfandRef;
    uint32_t lcbPlcfandRef;

    uint32_t fcPlcfandTxt;
    uint32_t lcbPlcfandTxt;

    uint32_t fcPlcfSed;
    uint32_t lcbPlcfSed;

    uint32_t ignore_fcPlcPad;
    uint32_t ignore_lcbPlcPad;

    uint32_t fcPlcfPhe;
    uint32_t lcbPlcfPhe;

    uint32_t fcSttbfGlsy;
    uint32_t lcbSttbfGlsy;

    uint32_t fcPlcfGlsy;
    uint32_t lcbPlcfGlsy;

    uint32_t fcPlcfHdd;
    uint32_t lcbPlcfHdd;

    uint32_t fcPlcfBteChpx;
    uint32_t lcbPlcfBteChpx;

    uint32_t fcPlcfBtePapx;
    uint32_t lcbPlcfBtePapx;

    uint32_t ignore_fcPlcfSea;
    uint32_t ignore_lcbPlcfSea;

    uint32_t fcSttbfFfn;
    uint32_t lcbSttbfFfn;

    uint32_t fcPlcfFldMom;
    uint32_t lcbPlcfFldMom;

    uint32_t fcPlcfFldHdr;
    uint32_t lcbPlcfFldHdr;

    uint32_t fcPlcfFldFtn;
    uint32_t lcbPlcfFldFtn;

    uint32_t fcPlcfFldAtn;
    uint32_t lcbPlcfFldAtn;

    uint32_t ignore_fcPlcfFldMcr;
    uint32_t ignore_lcbPlcfFldMcr;

    uint32_t fcSttbfBkmk;
    uint32_t lcbSttbfBkmk;

    uint32_t fcPlcfBkf;
    uint32_t lcbPlcfBkf;

    uint32_t fcPlcfBkl;
    uint32_t lcbPlcfBkl;

    uint32_t fcCmds;
    uint32_t lcbCmds;

    uint32_t ignore_fcUnused1;
    uint32_t ignore_lcbUnused1;

    uint32_t ignore_fcSttbfMcr;
    uint32_t ignore_lcbSttbfMcr;

    uint32_t fcPrDrvr;
    uint32_t lcbPrDrvr;

    uint32_t fcPrEnvPort;
    uint32_t lcbPrEnvPort;

    uint32_t fcPrEnvLand;
    uint32_t lcbPrEnvLand;

    uint32_t fcWss;
    uint32_t lcbWss;

    uint32_t fcDop;
    uint32_t lcbDop;

    uint32_t fcSttbfAssoc;
    uint32_t lcbSttbfAssoc;

    uint32_t fcClx;
    uint32_t lcbClx;

    uint32_t ignore_fcPlcfPgdFtn;
    uint32_t ignore_lcbPlcfPgdFtn;

    uint32_t ignore_fcAutosaveSource;
    uint32_t ignore_lcbAutosaveSource;

    uint32_t fcGrpXstAtnOwners;
    uint32_t lcbGrpXstAtnOwners;

    uint32_t fcSttbfAtnBkmk;
    uint32_t lcbSttbfAtnBkmk;

    uint32_t ignore_fcUnused2;
    uint32_t ignore_lcbUnused2;

    uint32_t ignore_fcUnused3;
    uint32_t ignore_lcbUnused3;

    uint32_t fcPlcSpaMom;
    uint32_t lcbPlcSpaMom;

    uint32_t fcPlcSpaHdr;
    uint32_t lcbPlcSpaHdr;

    uint32_t fcPlcfAtnBkf;
    uint32_t lcbPlcfAtnBkf;

    uint32_t fcPlcfAtnBkl;
    uint32_t lcbPlcfAtnBkl;

    uint32_t fcPms;
    uint32_t lcbPms;

    uint32_t ignore_fcFormFldSttbs;
    uint32_t ignore_lcbFormFldSttbs;

    uint32_t fcPlcfendRef;
    uint32_t lcbPlcfendRef;

    uint32_t fcPlcfendTxt;
    uint32_t lcbPlcfendTxt;

    uint32_t fcPlcfFldEdn;
    uint32_t lcbPlcfFldEdn;

    uint32_t ignore_fcUnused4;
    uint32_t ignore_lcbUnused4;

    uint32_t fcDggInfo;
    uint32_t lcbDggInfo;

    uint32_t fcSttbfRMark;
    uint32_t lcbSttbfRMark;

    uint32_t fcSttbfCaption;
    uint32_t lcbSttbfCaption;

    uint32_t fcSttbfAutoCaption;
    uint32_t lcbSttbfAutoCaption;

    uint32_t fcPlcfWkb;
    uint32_t lcbPlcfWkb;

    uint32_t fcPlcfSpl;
    uint32_t lcbPlcfSpl;

    uint32_t fcPlcftxbxTxt;
    uint32_t lcbPlcftxbxTxt;

    uint32_t fcPlcfFldTxbx;
    uint32_t lcbPlcfFldTxbx;

    uint32_t fcPlcfHdrtxbxTxt;
    uint32_t lcbPlcfHdrtxbxTxt;

    uint32_t fcPlcffldHdrTxbx;
    uint32_t lcbPlcffldHdrTxbx;

    uint32_t fcStwUser;
    uint32_t lcbStwUser;

    uint32_t fcSttbTtmbd;
    uint32_t lcbSttbTtmbd;

    uint32_t fcCookieData;
    uint32_t lcbCookieData;

    uint32_t fcPgdMotherOldOld;
    uint32_t lcbPgdMotherOldOld;

    uint32_t fcBkdMotherOldOld;
    uint32_t lcbBkdMotherOldOld;

    uint32_t fcPgdFtnOldOld;
    uint32_t lcbPgdFtnOldOld;

    uint32_t fcBkdFtnOldOld;
    uint32_t lcbBkdFtnOldOld;

    uint32_t fcPgdEdnOldOld;
    uint32_t lcbPgdEdnOldOld;

    uint32_t fcBkdEdnOldOld;
    uint32_t lcbBkdEdnOldOld;

    uint32_t ignore_fcSttbfIntlFld;
    uint32_t ignore_lcbSttbfIntlFld;

    uint32_t fcRouteSlip;
    uint32_t lcbRouteSlip;

    uint32_t fcSttbSavedBy;
    uint32_t lcbSttbSavedBy;

    uint32_t fcSttbFnm;
    uint32_t lcbSttbFnm;

    uint32_t fcPlfLst;
    uint32_t lcbPlfLst;

    uint32_t fcPlfLfo;
    uint32_t lcbPlfLfo;

    uint32_t fcPlcfTxbxBkd;
    uint32_t lcbPlcfTxbxBkd;

    uint32_t fcPlcfTxbxHdrBkd;
    uint32_t lcbPlcfTxbxHdrBkd;

    uint32_t fcDocUndoWord9;
    uint32_t lcbDocUndoWord9;

    uint32_t fcRgbUse;
    uint32_t lcbRgbUse;

    uint32_t fcUsp;
    uint32_t lcbUsp;

    uint32_t fcUskf;
    uint32_t lcbUskf;

    uint32_t fcPlcupcRgbUse;
    uint32_t lcbPlcupcRgbUse;

    uint32_t fcPlcupcUsp;
    uint32_t lcbPlcupcUsp;

    uint32_t fcSttbGlsyStyle;
    uint32_t lcbSttbGlsyStyle;

    uint32_t fcPlgosl;
    uint32_t lcbPlgosl;

    uint32_t fcPlcocx;
    uint32_t lcbPlcocx;

    uint32_t fcPlcfBteLvc;
    uint32_t lcbPlcfBteLvc;

    uint32_t dwLowDateTime;
    uint32_t dwHighDateTime;

    uint32_t fcPlcfLvcPre10;
    uint32_t lcbPlcfLvcPre10;

    uint32_t fcPlcfAsumy;
    uint32_t lcbPlcfAsumy;

    uint32_t fcPlcfGram;
    uint32_t lcbPlcfGram;

    uint32_t fcSttbListNames;
    uint32_t lcbSttbListNames;

    uint32_t fcSttbfUssr;
    uint32_t lcbSttbfUssr;
};

struct FIB_RgFcLcb2000 : public FIB_RgFcLcb97
{
    uint32_t fcPlcfTch;
    uint32_t lcbPlcfTch;

    uint32_t fcRmdThreading;
    uint32_t lcbRmdThreading;

    uint32_t ignore_fcMid;
    uint32_t ignore_lcbMid;

    uint32_t fcSttbRgtplc;
    uint32_t lcbSttbRgtplc;

    uint32_t fcMsoEnvelope;
    uint32_t lcbMsoEnvelope;

    uint32_t fcPlcfLad;
    uint32_t lcbPlcfLad;

    uint32_t fcRgDofr;
    uint32_t lcbRgDofr;

    uint32_t fcPlcosl;
    uint32_t lcbPlcosl;

    uint32_t fcPlcfCookieOld;
    uint32_t lcbPlcfCookieOld;

    uint32_t fcPgdMotherOld;
    uint32_t lcbPgdMotherOld;

    uint32_t fcBkdMotherOld;
    uint32_t lcbBkdMotherOld;

    uint32_t fcPgdFtnOld;
    uint32_t lcbPgdFtnOld;

    uint32_t fcBkdFtnOld;
    uint32_t lcbBkdFtnOld;

    uint32_t fcPgdEdnOld;
    uint32_t lcbPgdEdnOld;

    uint32_t fcBkdEdnOld;
    uint32_t lcbBkdEdnOld;
};

struct FIB_RgFcLcb2002 : public FIB_RgFcLcb2000
{
    uint32_t ignore_fcUnused1;
    uint32_t ignore_lcbUnused1;

    uint32_t fcPlcfPgp;
    uint32_t lcbPlcfPgp;

    uint32_t fcPlcfuim;
    uint32_t lcbPlcfuim;

    uint32_t fcPlfguidUim;
    uint32_t lcbPlfguidUim;

    uint32_t fcAtrdExtra;
    uint32_t lcbAtrdExtra;

    uint32_t fcPlrsid;
    uint32_t lcbPlrsid;

    uint32_t fcSttbfBkmkFactoid;
    uint32_t lcbSttbfBkmkFactoid;

    uint32_t fcPlcfBkfFactoid;
    uint32_t lcbPlcfBkfFactoid;

    uint32_t fcPlcfcookie;
    uint32_t lcbPlcfcookie;

    uint32_t fcPlcfBklFactoid;
    uint32_t lcbPlcfBklFactoid;

    uint32_t fcFactoidData;
    uint32_t lcbFactoidData;

    uint32_t fcDocUndo;
    uint32_t lcbDocUndo;

    uint32_t fcSttbfBkmkFcc;
    uint32_t lcbSttbfBkmkFcc;

    uint32_t fcPlcfBkfFcc;
    uint32_t lcbPlcfBkfFcc;

    uint32_t fcPlcfBklFcc;
    uint32_t lcbPlcfBklFcc;

    uint32_t fcSttbfbkmkBPRepairs;
    uint32_t lcbSttbfBkmkBPRepairs;

    uint32_t fcPlcfbkfBPRepairs;
    uint32_t lcbPlcfbkfBPRepairs;

    uint32_t fcPlcfbklBPRepairs;
    uint32_t lcbPlcfbklBPRepairs;

    uint32_t fcPmsNew;
    uint32_t lcbPmsNew;

    uint32_t fcODSO;
    uint32_t lcbODSO;

    uint32_t fcPlcfpmiOldXP;
    uint32_t lcbPlcfpmiOldXP;

    uint32_t fcPlcfpmiNewXP;
    uint32_t lcbPlcfpmiNewXP;

    uint32_t fcPlcfpmiMixedXP;
    uint32_t lcbPlcfpmiMixedXP;

    uint32_t ignore_fcUnused2;
    uint32_t ignore_lcbUnused2;

    uint32_t fcPlcffactoid;
    uint32_t lcbPlcffactoid;

    uint32_t fcPlcflvcOldXP;
    uint32_t lcbPlcflvcOldXP;

    uint32_t fcPlcflvcNewXP;
    uint32_t lcbPlcflvcNewXP;

    uint32_t fcPlcflvcMixedXP;
    uint32_t lcbPlcflvcMixedXP;
};

struct FIB_RgFcLcb2003 : public FIB_RgFcLcb2002
{
    uint32_t fcHplxsdr;
    uint32_t lcbHplxsdr;

    uint32_t fcSttbfBkmkSdt;
    uint32_t lcbSttbfBkmkSdt;

    uint32_t fcPlcfBkfSdt;
    uint32_t lcbPlcfBkfSdt;

    uint32_t fcPlcfBklSdt;
    uint32_t lcbPlcfBklSdt;

    uint32_t fcCustomXForm;
    uint32_t lcbCustomXForm;

    uint32_t fcSttbfBkmkProt;
    uint32_t lcbSttbfBkmkProt;

    uint32_t fcPlcfBkfProt;
    uint32_t lcbPlcfBkfProt;

    uint32_t fcPlcfBklProt;
    uint32_t lcbPlcfBklProt;

    uint32_t fcSttbProtUser;
    uint32_t lcbSttbProtUser;

    uint32_t ignore_fcUnused;
    uint32_t ignore_lcbUnused;

    uint32_t fcPlcfpmiOld;
    uint32_t lcbPlcfpmiOld;

    uint32_t fcPlcfpmiOldInline;
    uint32_t lcbPlcfpmiOldInline;

    uint32_t fcPlcfpmiNew;
    uint32_t lcbPlcfpmiNew;

    uint32_t fcPlcfpmiNewInline;
    uint32_t lcbPlcfpmiNewInline;

    uint32_t fcPlcflvcOld;
    uint32_t lcbPlcflvcOld;

    uint32_t fcPlcflvcOldInline;
    uint32_t lcbPlcflvcOldInline;

    uint32_t fcPlcflvcNew;
    uint32_t lcbPlcflvcNew;

    uint32_t fcPlcflvcNewInline;
    uint32_t lcbPlcflvcNewInline;

    uint32_t fcPgdMother;
    uint32_t lcbPgdMother;

    uint32_t fcBkdMother;
    uint32_t lcbBkdMother;

    uint32_t fcAfdMother;
    uint32_t lcbAfdMother;

    uint32_t fcPgdFtn;
    uint32_t lcbPgdFtn;

    uint32_t fcBkdFtn;
    uint32_t lcbBkdFtn;

    uint32_t fcAfdFtn;
    uint32_t lcbAfdFtn;

    uint32_t fcPgdEdn;
    uint32_t lcbPgdEdn;

    uint32_t fcBkdEdn;
    uint32_t lcbBkdEdn;

    uint32_t fcAfdEdn;
    uint32_t lcbAfdEdn;

    uint32_t fcAfd;
    uint32_t lcbAfd;
};

struct FIB_RgFcLcb2007 : public FIB_RgFcLcb2003
{
    uint32_t ignore_fcPlcfmthd;
    uint32_t ignore_lcbPlcfmthd;

    uint32_t ignore_fcSttbfBkmkMoveFrom;
    uint32_t ignore_lcbSttbfBkmkMoveFrom;

    uint32_t ignore_fcPlcfBkfMoveFrom;
    uint32_t ignore_lcbPlcfBkfMoveFrom;

    uint32_t ignore_fcPlcfBklMoveFrom;
    uint32_t ignore_lcbPlcfBklMoveFrom;

    uint32_t ignore_fcSttbfBkmkMoveTo;
    uint32_t ignore_lcbSttbfBkmkMoveTo;

    uint32_t ignore_fcPlcfBkfMoveTo;
    uint32_t ignore_lcbPlcfBkfMoveTo;

    uint32_t ignore_fcPlcfBklMoveTo;
    uint32_t ignore_lcbPlcfBklMoveTo;

    uint32_t ignore_fcUnused1;
    uint32_t ignore_lcbUnused1;

    uint32_t ignore_fcUnused2;
    uint32_t ignore_lcbUnused2;

    uint32_t ignore_fcUnused3;
    uint32_t ignore_lcbUnused3;

    uint32_t ignore_fcSttbfBkmkArto;
    uint32_t ignore_lcbSttbfBkmkArto;

    uint32_t ignore_fcPlcfBkfArto;
    uint32_t ignore_lcbPlcfBkfArto;

    uint32_t ignore_fcPlcfBklArto;
    uint32_t ignore_lcbPlcfBklArto;

    uint32_t ignore_fcArtoData;
    uint32_t ignore_lcbArtoData;

    uint32_t ignore_fcUnused4;
    uint32_t ignore_lcbUnused4;

    uint32_t ignore_fcUnused5;
    uint32_t ignore_lcbUnused5;

    uint32_t ignore_fcUnused6;
    uint32_t ignore_lcbUnused6;

    uint32_t ignore_fcOssTheme;
    uint32_t ignore_lcbOssTheme;

    uint32_t ignore_fcColorSchemeMapping;
    uint32_t ignore_lcbColorSchemeMapping;
};


struct FIB_RgCswNewData2000
{
    uint16_t cQuickSavesNew;
};

struct FIB_RgCswNewData2007 : public FIB_RgCswNewData2000
{
    uint16_t ignore_lidThemeOther;
    uint16_t ignore_lidThemeFE;
    uint16_t ignore_lidThemeCS;
};



struct Sprm
{
    unsigned ispmd : 9;
    unsigned A : 1;
    unsigned sgc : 3;
    unsigned spra : 3;
};

struct FcCompressed
{
    unsigned fc : 30;
    unsigned a : 1;
    unsigned b : 1;
};

struct Pcd
{
    struct Bits {
        unsigned A_fNoParaLast : 1;
        unsigned B_fR1 : 1;
        unsigned C_fDirty : 1;
        unsigned fR2 : 13;
    } bits;

    FcCompressed fc;

    uint16_t prm;
};

#endif // WBFF_STRUCTURES_H
