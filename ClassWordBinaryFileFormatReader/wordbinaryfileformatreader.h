#ifndef WORDBINARYFILEFORMATREADER_H
#define WORDBINARYFILEFORMATREADER_H

#include "ClassWindowsCompoundBinaryFileFormatReader/windowscompoundbinaryfileformatreader.h"
#include "ClassTextContainer/textcontainer.h"
#include "wbff_structures.h"
#include "ClassInputBinaryStream/inputbinarystream.h"

class WordBinaryFileFormatReader : protected WindowsCompoundBinaryFileFormatReader
{
private:
    struct FIB_Begin;
    //class InputBinaryStream;

    std::shared_ptr<FIB_Begin> _fibBegin;
    std::shared_ptr<InputBinaryStream> _wordDocumentStream;
    std::shared_ptr<InputBinaryStream> _tableStream;
    //struct FIB_RgFcLcb97;
    //struct FIB_RgFcLcb2000;
    //struct FIB_RgFcLcb2002;
    //struct FIB_RgFcLcb2003;
    //struct FIB_RgFcLcb2007;
    //FIB_Begin _fibBegin;

    //std::istream* _wordDocumentStream;
    //std::istream* _tableStream;

    std::vector<uint32_t> _charactersPositions;
    std::vector<Pcd> _charactersOffsets;

    std::vector<TextContainer> _textContainers;


    time_t _timeLastSave;
    std::string _timeLastSave_str;
    //time_t _timeOfModify;
public:
    WordBinaryFileFormatReader(const std::string& fileName);
    WordBinaryFileFormatReader(std::istream& stream);

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

    // Shit begin
    void readPrcArray(FIB_RgFcLcb97& fibEnd);
    void readPrcDataArray();
    void readPrlArray(int16_t prlArraySize);
    // Shit end

    // Next shit begin
    void readPcdtArray();
    void readPlcPcdArray(uint32_t plcPcdSize);
    void readCpArray(uint32_t lastCpValue);
    void readPlcArray();
    // Next shit end

    void readCharacters();
    void makeTextContainers(std::string charactersArray);

    void readDateTime(FIB_RgFcLcb97& fibEnd);
    time_t getTime(uint32_t dwLowDateTime, uint32_t dwHighDateTime);

    std::istream& getStream(std::string streamName);
};

#endif // WORDBINARYFILEFORMATREADER_H
