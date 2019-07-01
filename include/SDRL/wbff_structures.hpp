#ifndef WBFF_STRUCTURES_HPP
#define WBFF_STRUCTURES_HPP

#include <cinttypes>
#include <istream>

namespace sdrl {

// ///////////////////////////////////////////////////////////////////////// //
// ///////////////////////////////////////////////////////////////////////// //

/// The FIB_Base structure is the fixed-size portion of the Fib
struct FIB_Base {
    FIB_Base() = default;
    FIB_Base(std::istream& input);

    uint16_t fileIdentifier;    /// For Word document should be 0xA5EC
    uint16_t fileFormatVersion; /// Should be 0x00C1
    uint16_t languageID;
    uint16_t pnNext;            /// Offset of autotext elements

    uint32_t lKey;              /// Specifies the XOR obfuscation password verifier, if encrypted and have obfuscation
    uint16_t nFib;              /// Format version number
    uint8_t quickSavesCount;    /// If fileFormatVersion < 0x00D9 - amount of saves, else 0xF.

    bool isTemplateDocument;
    bool hasOnlyAutotext;
    bool lastSaveIsIncremental;
    bool hasPictures;
    bool isEncrypted;
    bool hasStreamTableOne;     /// Stream table id. If set true then use 1Table, else - 0Table.
    bool readOnlyRecomended;    /// Author recomended read only, if set true.
    bool hasPassword;           /// Has password for modify document, if set true.
    bool needOverrideInfo;      /// Need override information of language and font, if set true.
    bool fromFarEast;           /// Было ли приложение, создавшее документ - азиатским... СУКА, АЗИАТСКИМ, КАКОГО ХРЕНА?
    bool useXorObfuscation;     /// If _isEncrypted = true - use XOR obfuscation, if this set true.

    bool needLoadOverridePage;  /// Need override page properties, if set true.
};

/// overloaded operator for read data from input stream
std::istream& operator >> (std::istream& istream, FIB_Base& fib);

// ///////////////////////////////////////////////////////////////////////// //
// ///////////////////////////////////////////////////////////////////////// //

struct FIB_RgW97
{
    uint16_t lidFE;
};

/// overloaded operator for read data from input stream
std::istream& operator >> (std::istream& istream, FIB_RgW97& fib);

// ///////////////////////////////////////////////////////////////////////// //
// ///////////////////////////////////////////////////////////////////////// //

struct FIB_RgLw97
{
    uint32_t cbMac;

    int32_t ccpText;
    int32_t ccpFtn;
    int32_t ccpHdd;

    int32_t ccpAtn;
    int32_t ccpEdn;
    int32_t ccpTxbx;
    int32_t ccpHdrTxbx;
};

/// overloaded operator for read data from input stream
std::istream& operator >> (std::istream& istream, FIB_RgLw97& fib);

// ///////////////////////////////////////////////////////////////////////// //
// ///////////////////////////////////////////////////////////////////////// //

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

/// overloaded operator for read data from input stream
std::istream& operator >> (std::istream& istream, FIB_RgFcLcb97& fib);

// ///////////////////////////////////////////////////////////////////////// //
// ///////////////////////////////////////////////////////////////////////// //

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

/// overloaded operator for read data from input stream
std::istream& operator >> (std::istream& istream, FIB_RgFcLcb2000& fib);

// ///////////////////////////////////////////////////////////////////////// //
// ///////////////////////////////////////////////////////////////////////// //

struct FIB_RgFcLcb2002 : public FIB_RgFcLcb2000
{
    uint32_t ignore_fcUnused1_2002;
    uint32_t ignore_lcbUnused1_2002;

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

    uint32_t ignore_fcUnused2_2002;
    uint32_t ignore_lcbUnused2_2002;

    uint32_t fcPlcffactoid;
    uint32_t lcbPlcffactoid;

    uint32_t fcPlcflvcOldXP;
    uint32_t lcbPlcflvcOldXP;

    uint32_t fcPlcflvcNewXP;
    uint32_t lcbPlcflvcNewXP;

    uint32_t fcPlcflvcMixedXP;
    uint32_t lcbPlcflvcMixedXP;
};

/// overloaded operator for read data from input stream
std::istream& operator >> (std::istream& istream, FIB_RgFcLcb2002& fib);

// ///////////////////////////////////////////////////////////////////////// //
// ///////////////////////////////////////////////////////////////////////// //

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

/// overloaded operator for read data from input stream
std::istream& operator >> (std::istream& istream, FIB_RgFcLcb2003& fib);

// ///////////////////////////////////////////////////////////////////////// //
// ///////////////////////////////////////////////////////////////////////// //

/**
 * @brief The FIB_RgFcLcb2007 struct for .doc's with FIB type 2007 year
 * @warning All fields shuld be ignored (According to the documentation)
 */
struct FIB_RgFcLcb2007 : public FIB_RgFcLcb2003
{
    uint32_t fcPlcfmthd;
    uint32_t lcbPlcfmthd;

    uint32_t fcSttbfBkmkMoveFrom;
    uint32_t lcbSttbfBkmkMoveFrom;

    uint32_t fcPlcfBkfMoveFrom;
    uint32_t lcbPlcfBkfMoveFrom;

    uint32_t fcPlcfBklMoveFrom;
    uint32_t lcbPlcfBklMoveFrom;

    uint32_t fcSttbfBkmkMoveTo;
    uint32_t lcbSttbfBkmkMoveTo;

    uint32_t fcPlcfBkfMoveTo;
    uint32_t lcbPlcfBkfMoveTo;

    uint32_t fcPlcfBklMoveTo;
    uint32_t lcbPlcfBklMoveTo;

    uint32_t fcUnused1;
    uint32_t lcbUnused1;

    uint32_t fcUnused2;
    uint32_t lcbUnused2;

    uint32_t fcUnused3;
    uint32_t lcbUnused3;

    uint32_t fcSttbfBkmkArto;
    uint32_t lcbSttbfBkmkArto;

    uint32_t fcPlcfBkfArto;
    uint32_t lcbPlcfBkfArto;

    uint32_t fcPlcfBklArto;
    uint32_t lcbPlcfBklArto;

    uint32_t fcArtoData;
    uint32_t lcbArtoData;

    uint32_t fcUnused4;
    uint32_t lcbUnused4;

    uint32_t fcUnused5;
    uint32_t lcbUnused5;

    uint32_t fcUnused6;
    uint32_t lcbUnused6;

    uint32_t fcOssTheme;
    uint32_t lcbOssTheme;

    uint32_t fcColorSchemeMapping;
    uint32_t lcbColorSchemeMapping;
};

/// overloaded operator for read data from input stream
std::istream& operator >> (std::istream& istream, FIB_RgFcLcb2007& fib);

// ///////////////////////////////////////////////////////////////////////// //
// ///////////////////////////////////////////////////////////////////////// //


struct FIB_RgCswNewData2000
{
    uint16_t cQuickSavesNew;
};

/// overloaded operator for read data from input stream
std::istream& operator >> (std::istream& istream, FIB_RgCswNewData2000& fib);

// ///////////////////////////////////////////////////////////////////////// //
// ///////////////////////////////////////////////////////////////////////// //

struct FIB_RgCswNewData2007 : public FIB_RgCswNewData2000
{
    uint16_t ignore_lidThemeOther;
    uint16_t ignore_lidThemeFE;
    uint16_t ignore_lidThemeCS;
};

/// overloaded operator for read data from input stream
std::istream& operator >> (std::istream& istream, FIB_RgCswNewData2007& fib);

// ///////////////////////////////////////////////////////////////////////// //
// ///////////////////////////////////////////////////////////////////////// //


struct Sprm
{
    uint16_t ispmd;
    bool f;
    uint8_t sgc;
    uint8_t spra;

    Sprm(uint16_t sprm) :
        ispmd( sprm & 0x01FF ),
        f ( (sprm >> 9) & 0x0001 ),
        sgc ( (sprm >> 10) & 0x0007 ),
        spra ( sprm >> 13 ) {}
};


struct Pcd
{
    bool A_fNoParaLast;
    bool B_fR1;
    bool C_fDirty;
    uint16_t fR2;

    uint32_t fc;
    bool fc_A_fCompressed;
    bool fc_B_r1;

    bool prm_A_fComplex;
    uint16_t prm_data;
};

}

#endif // WBFF_STRUCTURES_H
