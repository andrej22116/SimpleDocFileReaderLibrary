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

    std::shared_ptr<FIB_Begin> _fibBegin;
    std::shared_ptr<InputBinaryStream<char>> _wordDocumentStream;
    std::shared_ptr<InputBinaryStream<char>> _tableStream;
	std::shared_ptr<FIB_RgFcLcb97> _fibEnd;

    time_t _timeLastSave;
    std::string _timeLastSave_str;
    //time_t _timeOfModify;
public:
	WordBinaryDocumentReader(const std::wstring& fileName, uint16_t flags = WBFF_Texts);
	WordBinaryDocumentReader(std::istream& stream, uint16_t flags = WBFF_Texts);

    time_t getLastSaveTime() { return _timeLastSave; }
    std::string getLastSaveTime_str() { return _timeLastSave_str; }

	/** return format year: 1997, 2000, 2002, 2003, 2007 */
	uint16_t formatVersion();

	std::wstring text();

	std::vector<uint32_t> sections();

private:
    void readDocumentHeader();


    /// Work with document date and time! Begin
    void readDateTime(FIB_RgFcLcb97& fibEnd);
    time_t getTime(uint32_t dwLowDateTime, uint32_t dwHighDateTime);
    /// Work with document date and time! End

    std::istream& getStream(std::string streamName);
};

}

#endif // SDRL_H
