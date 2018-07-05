#ifndef WORDBINARYFILEFORMATREADER_H
#define WORDBINARYFILEFORMATREADER_H

#include "ClassWindowsCompoundBinaryFileFormatReader/windowscompoundbinaryfileformatreader.h"
#include "wbff_structures.h"
#include "ClassInputBinaryStream/inputbinarystream.h"

#include "Containers/ClassTextContainer/textcontainer.h"
#include "Containers/ClassTableContainer/tablecontainer.h"
#include "Containers/ClassImageContainer/imagecontainer.h"

#include <stack>

class WordBinaryFileFormatReader : protected WindowsCompoundBinaryFileFormatReader
{
public:
    enum WBFF_Objects
    {
        WBFF_Texts = 1,
        WBFF_Tables = 2,
        WBFF_Images = 4,
    };

private:
    enum WBFF_Marks : uint16_t
    {
        Mark_Table = 0x7,
        Mark_Image = 0x8,
        Mark_Paragraph = 0x13
    };

    uint16_t _flags;
    struct ReadFlags;

    struct FIB_Begin;
    struct CLX_Data;
    struct Parse_Data;

    std::shared_ptr<FIB_Begin> _fibBegin;
    std::shared_ptr<InputBinaryStream> _wordDocumentStream;
    std::shared_ptr<InputBinaryStream> _tableStream;

    /// For parse characters blocks;
    //std::wstringstream characterStream;
    std::wstringstream _characterStream;
    //int32_t _lastParagraphSymbolPos = -1;
    //int32_t _lastTableSymbolPos = -1;
    //bool _hasParagraphSymbol = false;
    //bool _hasTableSymbol = false;
    /// End. :)

    /// Experement with PlcBtePapx begin
    /// Result
    std::vector<uint32_t> _offsetsOfDataDiapasons;
    std::vector<uint32_t> _offsetsOfSequenceStructuresWithCharactersDiapasons;
    std::vector<uint32_t> _characterDiapasons;
    //std::vector<std::vector<uint32_t>> _characterDiapasons;
    //std::map<uint32_t, std::vector<uint32_t>> _cpDiapasons;
    /// Experement
    std::vector<uint32_t> _plcBtePapx_afc;
    std::vector<uint32_t> _plcBtePapx_pnFkpPapx;
    //std::vector<std::vector<uint32_t>> _plcBtePapx_PapxFkp;
    std::vector<uint32_t> _plcBtePapx_PapxFkp;
    void readAFC(FIB_RgFcLcb97& fibEnd);
    void readPnFkpPapx();
    void readPapxFkp();
    /// End

    std::vector<uint32_t> _charactersPositions;
    std::vector<Pcd> _charactersOffsets;

    /// Containers
    std::vector<std::shared_ptr<Container>> _allContainers;
    //std::vector<TextContainer> _textContainers;
    //std::vector<TableContainer> _tableContainers;
    //std::vector<ImageContainer> _imageContainers;
    //std::shared_ptr<Container> lastContainer;
    /// End


    time_t _timeLastSave;
    std::string _timeLastSave_str;
    //time_t _timeOfModify;
public:
    WordBinaryFileFormatReader(const std::string& fileName, uint16_t flags = WBFF_Texts);
    WordBinaryFileFormatReader(std::istream& stream, uint16_t flags = WBFF_Texts);

    inline time_t getLastSaveTime() { return _timeLastSave; }
    inline std::string getLastSaveTime_str() { return _timeLastSave_str; }

    inline const std::vector<TextContainer>& getContainers();

private:
    void readDocument();

    void readDocument97(FIB_RgFcLcb97& fibEnd);
    void readDocument00(FIB_RgFcLcb2000& fibEnd);
    void readDocument02(FIB_RgFcLcb2002& fibEnd);
    void readDocument03(FIB_RgFcLcb2003& fibEnd);
    void readDocument07(FIB_RgFcLcb2007& fibEnd);

    void readClxArray(FIB_RgFcLcb97& fibEnd);

    /// Reading data for find characters! Begin
    // Удалить это! Начало
    void readPrcArray(FIB_RgFcLcb97& fibEnd);
    void readPrcDataArray();
    void readPrlArray(int16_t prlArraySize);

    void readPrcArray(FIB_RgFcLcb97& fibEnd, CLX_Data& clxData);
    void readPrcDataArray(CLX_Data& clxData);
    void readPrlArray(CLX_Data& clxData);
    // Удалить это! Конец

    void readPcdtArray();
    void readPlcPcdArray(uint32_t plcPcdSize);
    void readCpArray(uint32_t lastCpValue);
    void readPlcArray();

    void readPcdtArray(CLX_Data& clxData);
    void readPlcPcdArray(CLX_Data& clxData);
    void readCpArray(CLX_Data& clxData);
    void readPlcArray(CLX_Data& clxData);
    /// Reading data for find characters! End

    /// Reading characters diapasons! Begin
    void readCpDiapasons(FIB_RgFcLcb97& fibEnd);
    void readOffsetsOfDataDiapasons(FIB_RgFcLcb97& fibEnd);
    void readOffsetsOfSequenceStructuresWithCharactersDiapasons();
    void readCharacterDiapasons();
    /// Reading characters diapasons! End

    /// Make containers! Begin
    void makeContainers();
    void modifyDiapasonsForWorkWithCharacterStream();
    void readCharactersAndCreateContainers();
    void readCharacters();
    void readCharacters(CLX_Data& clxData);
    /// Make containers! End

    /// Work with document date and time! Begin
    void readDateTime(FIB_RgFcLcb97& fibEnd);
    time_t getTime(uint32_t dwLowDateTime, uint32_t dwHighDateTime);
    /// Work with document date and time! End

    std::istream& getStream(std::string streamName);
};

#endif // WORDBINARYFILEFORMATREADER_H
