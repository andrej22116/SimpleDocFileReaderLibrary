#include "wordbinaryfileformatreader.h"
//#include "ClassInputBinaryStream/inputbinarystream.h"

//#include "wbff_structures.h"

struct WordBinaryFileFormatReader::FIB_Begin
{
    FIB_Base fibBase;
    FIB_RgW97 fibRgW97;
    FIB_RgLw97 fibLw97;
};



WordBinaryFileFormatReader::WordBinaryFileFormatReader(const std::string& fileName)
    : WindowsCompoundBinaryFileFormatReader(fileName)
{
    readDocument();
}

WordBinaryFileFormatReader::WordBinaryFileFormatReader(std::istream& stream)
    : WindowsCompoundBinaryFileFormatReader(stream)
{
    readDocument();
}


void WordBinaryFileFormatReader::readDocument()
{
    _wordDocumentStream = std::make_shared<InputBinaryStream>(getStream("WordDocument"));

    _fibBegin = std::make_shared<FIB_Begin>();
    _fibBegin->fibBase = _wordDocumentStream->getData<FIB_Base>();
    _wordDocumentStream->getData<uint16_t>();
    _fibBegin->fibRgW97 = _wordDocumentStream->getData<FIB_RgW97>();
    _wordDocumentStream->getData<uint16_t>();
    _fibBegin->fibLw97 = _wordDocumentStream->getData<FIB_RgLw97>();
    _wordDocumentStream->getData<uint16_t>();

    std::string nameofTableStream("0Table");
    if (_fibBegin->fibBase.G_fWhichTblStm == 1)
    {
        nameofTableStream = "1Table";
    }

    _tableStream = std::make_shared<InputBinaryStream>(getStream(nameofTableStream));

    switch (_fibBegin->fibBase.fileFormatVersion)
    {
    case 0x00C1: {
        std::cout << "File FIB version: 1997!\n" << std::endl;
        auto fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb97>();
        readDocument97(fibEnd);
    } break;
    case 0x00D9: {
        std::cout << "File FIB version: 2000!\n" << std::endl;
        auto fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb2000>();
        readDocument00(fibEnd);
    } break;
    case 0x0101: {
        std::cout << "File FIB version: 2002!\n" << std::endl;
        auto fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb2002>();
        readDocument02(fibEnd);
    } break;
    case 0x010C: {
        std::cout << "File FIB version: 2003!\n" << std::endl;
        auto fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb2003>();
        readDocument03(fibEnd);
    } break;
    case 0x0112: {
        std::cout << "File FIB version: 2007!\n" << std::endl;
        auto fibEnd = _wordDocumentStream->getData<FIB_RgFcLcb2007>();
        readDocument07(fibEnd);
    } break;
    }
}


void WordBinaryFileFormatReader::readDocument97(FIB_RgFcLcb97& fibEnd)
{
    // Read date for document 1997
    std::cout << "Read version: 1997!" << std::endl;

    std::cout << "FibLw97 ccpText: " << _fibBegin->fibLw97.ccpText << std::endl;
    std::cout << "FibEnd fcClx: " << fibEnd.fcClx << std::endl;
    std::cout << "FibEnd lcbClx: " << fibEnd.lcbClx << std::endl;

    readClxArray(fibEnd);

    std::cout << std::endl;
}


void WordBinaryFileFormatReader::readDocument00(FIB_RgFcLcb2000& fibEnd)
{
    // Read date for document 2000
    readDocument97(fibEnd);
    std::cout << "Read version: 2000!" << std::endl;
}


void WordBinaryFileFormatReader::readDocument02(FIB_RgFcLcb2002& fibEnd)
{
    // Read date for document 2002
    readDocument00(fibEnd);
    std::cout << "Read version: 2002!" << std::endl;
}


void WordBinaryFileFormatReader::readDocument03(FIB_RgFcLcb2003& fibEnd)
{
    // Read date for document 2003
    readDocument02(fibEnd);
    std::cout << "Read version: 2003!" << std::endl;
}


void WordBinaryFileFormatReader::readDocument07(FIB_RgFcLcb2007& fibEnd)
{
    // Read date for document 2007. No data. :3
    readDocument03(fibEnd);
    std::cout << "Read version: 2007!" << std::endl;
}



void WordBinaryFileFormatReader::readClxArray(FIB_RgFcLcb97& fibEnd)
{
    readPrcArray(fibEnd);
    readPcdtArray(fibEnd);
}


void WordBinaryFileFormatReader::readPrcArray(FIB_RgFcLcb97& fibEnd)
{
    _tableStream->seekg(fibEnd.fcClx, _tableStream->beg);

    while(_tableStream->peekData<uint8_t>() == 0x01)
    {
        _tableStream->getData<uint8_t>();
        readPrcDataArray();
    }
}


void WordBinaryFileFormatReader::readPrcDataArray()
{
    std::cout << "Hello_1!" << std::endl;
    auto cbGrpprl = _tableStream->getData<int16_t>();
    readPrlArray(cbGrpprl);
}


void WordBinaryFileFormatReader::readPrlArray(int16_t prlArraySize)
{
    static const uint16_t bufferSize = 1024;
    static char buffer[bufferSize];
    //auto sprm = _tableStream->getData<Sprm>();

    // Заглушка
    for (uint16_t needReadBytes = prlArraySize; needReadBytes > 0;)
    {
        uint16_t bytesAmountForRead = needReadBytes < bufferSize ? needReadBytes : bufferSize;
        needReadBytes -= bytesAmountForRead;

        _tableStream->read(buffer, bytesAmountForRead);
    }
}




void WordBinaryFileFormatReader::readPcdtArray(FIB_RgFcLcb97& fibEnd)
{
    while(_tableStream->peekData<uint8_t>() == 0x02)
    {
        _tableStream->getData<uint8_t>();
        readPlcPcdArray(_tableStream->getData<uint32_t>());
    }
}


void WordBinaryFileFormatReader::readPlcPcdArray(uint32_t plcPcdSize)
{
    std::cout << "Hello_2! plcPcdSize: " << plcPcdSize << std::endl;
    _charactersPositions.reserve(plcPcdSize / sizeof(uint32_t));

    FIB_RgLw97& fibLw = _fibBegin->fibLw97;
    uint32_t lastCP = fibLw.ccpText;
    uint32_t ccpSumm = fibLw.ccpAtn + fibLw.ccpEdn + fibLw.ccpFtn + fibLw.ccpHdd + fibLw.ccpHdrTxbx + fibLw.ccpTxbx;
    if (ccpSumm > 0)
    {
        lastCP += ccpSumm + 1;
    }

    readCpArray(lastCP);
    readPlcArray();
    //_tableStream->seekg(_tableStream->tellg() + plcPcdSize, _tableStream->beg);
}


void WordBinaryFileFormatReader::readCpArray(uint32_t lastCpValue)
{
    uint32_t cpOffset = 0;
    do
    {
        cpOffset = _tableStream->getData<uint32_t>();
        _charactersPositions.push_back(cpOffset);
    }
    while(cpOffset != lastCpValue);
}


void WordBinaryFileFormatReader::readPlcArray()
{
    for (uint32_t i = 1; i < _charactersPositions.size(); i++)
    {
        Pcd pcd;
        pcd.bits = _tableStream->getData<Pcd::Bits>();
        pcd.fc = _tableStream->getData<FcCompressed>();
        pcd.prm = _tableStream->getData<uint16_t>();
        _charactersOffsets.push_back(pcd);
    }
}


void WordBinaryFileFormatReader::makeStrings()
{

}



std::istream& WordBinaryFileFormatReader::getStream(std::string streamName)
{
    auto streamID = this->getStreamIdByName(streamName);
    if (streamID < 0)
    {
        throw std::runtime_error("Stream \"" + streamName + "\" not found!");
    }

    return this->getStreamById(streamID);
}
