#ifndef W_BFFR_H
#define W_BFFR_H

#include <SDRL/DocDocument/Compound_wcbff_structures.h>
#include <SDRL/DocDocument/WordBinaryFileFormat/wbff_structures.h>
#include <SDRL/DocDocument/inputbinarystream.h>
#include <SDRL/DocDocument/CompoundBFFR.hpp>

#include <sstream>

namespace sdrl {

class WordBinaryDocumentReader : protected WindowsCompoundBinaryFileFormatReader
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
        Mark_Paragraph = 0xD
    };

    uint16_t _flags;
    struct ReadFlags;

    struct FIB_Begin;
    struct CLX_Data;
    struct Parse_Data;

    std::shared_ptr<FIB_Begin> _fibBegin;
    std::shared_ptr<InputBinaryStream<char>> _wordDocumentStream;
    std::shared_ptr<InputBinaryStream<char>> _tableStream;

    /// For parse characters blocks
    std::wstringstream _characterStream;

    /// CpDiapasons Begin
    std::vector<uint32_t> _offsetsOfDataDiapasons;
    std::vector<uint32_t> _offsetsOfSequenceStructuresWithCharactersDiapasons;
    std::vector<uint8_t>  _characterDiapasonsPropertiesOffsets;
    std::vector<uint16_t> _characterDiapasonsStyles;
    std::vector<Sprm>     _characterDiapasonsModify;
    std::vector<uint32_t> _characterDiapasonsModifyOperands;
    std::vector<uint32_t> _characterDiapasons;
    /// CpDiapasons End

    std::vector<uint32_t> _charactersPositions;
    std::vector<Pcd> _charactersOffsets;

    time_t _timeLastSave;
    std::string _timeLastSave_str;
    //time_t _timeOfModify;
public:
	WordBinaryDocumentReader(const std::wstring& fileName, uint16_t flags = WBFF_Texts);
	WordBinaryDocumentReader(std::istream& stream, uint16_t flags = WBFF_Texts);

    time_t getLastSaveTime() { return _timeLastSave; }
    std::string getLastSaveTime_str() { return _timeLastSave_str; }

	std::wstring text() { return _characterStream.str(); }

private:
    void readDocument();

    void readDocument97(FIB_RgFcLcb97& fibEnd);
    void readDocument00(FIB_RgFcLcb2000& fibEnd);
    void readDocument02(FIB_RgFcLcb2002& fibEnd);
    void readDocument03(FIB_RgFcLcb2003& fibEnd);
    void readDocument07(FIB_RgFcLcb2007& fibEnd);

    void readClxArray(FIB_RgFcLcb97& fibEnd);

    /// Reading data for find characters! Begin
    //void readText(FIB_RgFcLcb97& fibEnd);   // Need doing this!
    // Удалить это! Начало
    void readPrcArray(FIB_RgFcLcb97& fibEnd);
    void readPrcDataArray();
    void readPrlArray(int16_t prlArraySize);
    // Удалить это! Конец
	/*
    void readPrcArray(FIB_RgFcLcb97& fibEnd, CLX_Data& clxData);
    void readPrcDataArray(CLX_Data& clxData);
    void readPrlArray(CLX_Data& clxData);
	*/
    // Удалить это! Начало
    void readPcdtArray();
    void readPlcPcdArray(uint32_t plcPcdSize);
    void readCpArray(uint32_t lastCpValue);
    void readPlcArray();
    // Удалить это! Конец
	/*
    void readPcdtArray(CLX_Data& clxData);
    void readPlcPcdArray(CLX_Data& clxData);
    void readCpArray(CLX_Data& clxData);
    void readPlcArray(CLX_Data& clxData);
	*/
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
    //void readCharacters(CLX_Data& clxData);
    /// Make containers! End

    /// Work with document date and time! Begin
    void readDateTime(FIB_RgFcLcb97& fibEnd);
    time_t getTime(uint32_t dwLowDateTime, uint32_t dwHighDateTime);
    /// Work with document date and time! End

    std::istream& getStream(std::string streamName);
};

}

#endif // SDRL_H
