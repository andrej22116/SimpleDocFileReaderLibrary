#include <SDRL/wordBFFR.h>//"SDRL.h"
#include <SDRL/variablevisualize.hpp>//"variablevisualize.hpp"
//#include "ClassInputBinaryStream/inputbinarystream.h"

//#include "wbff_structures.h"

struct SDRL::FIB_Begin
{
    FIB_Base fibBase;
    FIB_RgW97 fibRgW97;
    FIB_RgLw97 fibLw97;
};


struct SDRL::CLX_Data {
    std::vector<uint32_t> characterSequences;
    std::vector<Pcd> characterSequencesOffsets;

    int16_t prlArraySize;

    uint32_t plcPcdSize;
    uint32_t lastCpValue;
};



SDRL::SDRL(const std::string& fileName, uint16_t flags)
    : WindowsCompoundBinaryFileFormatReader(fileName), _flags(flags)
{
    readDocument();
}

SDRL::SDRL(std::istream& stream, uint16_t flags)
    : WindowsCompoundBinaryFileFormatReader(stream), _flags(flags)
{
    readDocument();
}


void SDRL::readDocument()
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

    clearData();
    _wordDocumentStream.reset();
    _tableStream.reset();
    _fibBegin.reset();
}


void SDRL::readDocument97(FIB_RgFcLcb97& fibEnd)
{
    // Read date for document 1997
    std::cout << "Read version: 1997!" << std::endl;

    readCpDiapasons(fibEnd);

    readClxArray(fibEnd);
    readCharacters();

    makeContainers();

    readDateTime(fibEnd);

    std::cout << std::endl;
}


void SDRL::readDocument00(FIB_RgFcLcb2000& fibEnd)
{
    // Read date for document 2000
    readDocument97(fibEnd);
    std::cout << "Read version: 2000!" << std::endl;
}


void SDRL::readDocument02(FIB_RgFcLcb2002& fibEnd)
{
    // Read date for document 2002
    readDocument00(fibEnd);
    std::cout << "Read version: 2002!" << std::endl;
}


void SDRL::readDocument03(FIB_RgFcLcb2003& fibEnd)
{
    // Read date for document 2003
    readDocument02(fibEnd);
    std::cout << "Read version: 2003!" << std::endl;
}


void SDRL::readDocument07(FIB_RgFcLcb2007& fibEnd)
{
    // Read date for document 2007. No data. :3
    readDocument03(fibEnd);
    std::cout << "Read version: 2007!" << std::endl;
}



void SDRL::readClxArray(FIB_RgFcLcb97& fibEnd)
{
    readPrcArray(fibEnd);
    readPcdtArray();
}


void SDRL::readPrcArray(FIB_RgFcLcb97& fibEnd)
{
    _tableStream->seekg(fibEnd.fcClx, _tableStream->beg);

    while(_tableStream->peekData<uint8_t>() == 0x01)
    {
        _tableStream->getData<uint8_t>();
        readPrcDataArray();
    }
}


void SDRL::readPrcDataArray()
{
    auto cbGrpprl = _tableStream->getData<int16_t>();
    readPrlArray(cbGrpprl);
}


void SDRL::readPrlArray(int16_t prlArraySize)
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




void SDRL::readPcdtArray()
{
    while(_tableStream->peekData<uint8_t>() == 0x02)
    {
        _tableStream->getData<uint8_t>();
        readPlcPcdArray(_tableStream->getData<uint32_t>());
    }
}


void SDRL::readPlcPcdArray(uint32_t plcPcdSize)
{
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
}


void SDRL::readCpArray(uint32_t lastCpValue)
{
    uint32_t cpOffset = 0;
    do
    {
        cpOffset = _tableStream->getData<uint32_t>();
        _charactersPositions.push_back(cpOffset);
    }
    while(cpOffset != lastCpValue);
}


void SDRL::readPlcArray()
{
    for (uint32_t i = 1; i < _charactersPositions.size(); i++)
    {
        Pcd pcd;
        pcd.bits = _tableStream->getData<uint16_t>();
        pcd.fc = _tableStream->getData<FcCompressed>();
        pcd.prm = _tableStream->getData<uint16_t>();
        _charactersOffsets.push_back(pcd);
    }
}


void SDRL::readCharacters()
{
    for (uint32_t i = 0, size = _charactersPositions.size() - 1; i < size; i++)
    {
        uint32_t symbolsAmount = _charactersPositions[i + 1] - _charactersPositions[i];

        if (_charactersOffsets[i].fc.a == 0)
        {
            std::u16string str(symbolsAmount, 0);
            _wordDocumentStream->seekg(_charactersOffsets[i].fc.fc, _wordDocumentStream->beg);
            _wordDocumentStream->read((char*)str.data(), sizeof(char16_t) * symbolsAmount);

            std::wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>> conversion;
            _characterStream << conversion.from_bytes(
                        reinterpret_cast<const char*> (&str[0]),
                        reinterpret_cast<const char*> (&str[0] + str.size()));
            int a = 0; a++;
        }
        else
        {
            std::string str(symbolsAmount, 0);
            _wordDocumentStream->seekg(_charactersOffsets[i].fc.fc / 2, _wordDocumentStream->beg);
            _wordDocumentStream->read((char*)str.data(), symbolsAmount);
            _characterStream << std::wstring(str.begin(), str.end());
        }
    }
}


void SDRL::readDateTime(FIB_RgFcLcb97& fibEnd)
{
    _timeLastSave = getTime(fibEnd.dwLowDateTime, fibEnd.dwHighDateTime);
    std::tm* tm = std::localtime(&_timeLastSave);
    std::stringstream ss;
    ss << std::put_time(tm, "Date: %F; Time: %T");
    _timeLastSave_str = ss.str();
}


time_t SDRL::getTime(uint32_t dwLowDateTime, uint32_t dwHighDateTime)
{
    static const uint64_t EPOCH_DIFFERENCE_MICROS = 11644473600LL;
    uint64_t total_us = ((uint64_t)dwHighDateTime << 32) + dwLowDateTime;
    //total_us -= EPOCH_DIFFERENCE_MICROS;

    return (time_t)(total_us / 10000000 - EPOCH_DIFFERENCE_MICROS);
}

std::istream& SDRL::getStream(std::string streamName)
{
    auto streamID = this->getStreamIdByName(streamName);
    if (streamID < 0)
    {
        throw std::runtime_error("Stream \"" + streamName + "\" not found!");
    }

    return this->getStreamById(streamID);
}


///////////////////////////////////////////
/// Reading characters diapasons! Begin ///
///////////////////////////////////////////

void SDRL::readCpDiapasons(FIB_RgFcLcb97& fibEnd)
{
    readOffsetsOfDataDiapasons(fibEnd);
    readOffsetsOfSequenceStructuresWithCharactersDiapasons();
    readCharacterDiapasons();
}


void SDRL::readOffsetsOfDataDiapasons(FIB_RgFcLcb97& fibEnd)
{
    _tableStream->seekg(fibEnd.fcPlcfBtePapx, _wordDocumentStream->beg);
    uint32_t offset = 0;
    while ( _tableStream->peekData<uint32_t>() > offset)
    {
        offset = _tableStream->getData<uint32_t>();
        _offsetsOfDataDiapasons.push_back(offset);
    }
}


void SDRL::readOffsetsOfSequenceStructuresWithCharactersDiapasons()
{
    for (int i = 0, size = _offsetsOfDataDiapasons.size() - 1; i < size; i++)
    {
        uint32_t offset = _tableStream->getData<uint32_t>();
        offset <<= 10;
        offset >>= 10;
        _offsetsOfSequenceStructuresWithCharactersDiapasons.push_back(offset * 512);
    }
}


void SDRL::readCharacterDiapasons()
{
    for (auto offset : _offsetsOfSequenceStructuresWithCharactersDiapasons)
    {
        _wordDocumentStream->seekg(offset, _wordDocumentStream->beg);
        auto rgfcAmount = _wordDocumentStream->peekData<uint8_t>(offset + 511);

        if (_characterDiapasons.empty())
        {
            _characterDiapasons.push_back(_wordDocumentStream->getData<uint32_t>());
        }
        else
        {
            _wordDocumentStream->ignore(sizeof(uint32_t));
        }
        for (int i = 0; i < rgfcAmount; i++)
        {
            _characterDiapasons.push_back(_wordDocumentStream->getData<uint32_t>());
        }

        for (int i = 0; i < rgfcAmount; i++)
        {
            _characterDiapasonsPropertiesOffsets.push_back(_wordDocumentStream->getData<uint8_t>());
            _wordDocumentStream->ignore(sizeof(uint32_t) * 3);
        }

        for (uint16_t structOffset : _characterDiapasonsPropertiesOffsets)
        {
            if (structOffset == 0) continue;

            structOffset *= 2;
            _wordDocumentStream->seekg(offset + structOffset, _wordDocumentStream->beg);

            uint16_t structSize = _wordDocumentStream->getData<uint8_t>();
            if (structSize == 0)
            {
                structSize = _wordDocumentStream->getData<uint8_t>() * 2;
            }
            else
            {
                structSize = (structSize * 2) - 1;
            }

            _characterDiapasonsStyles.push_back(_wordDocumentStream->getData<uint16_t>());
            for (int bytesReaded = sizeof(uint16_t); bytesReaded < structSize;)
            {
                Sprm sprm(_wordDocumentStream->getData<uint16_t>());
                _characterDiapasonsModify.push_back(sprm);
                bytesReaded += sizeof(uint16_t);

                switch (sprm.spra)
                {
                case 0: case 1: {
                    _characterDiapasonsModifyOperands.push_back(_wordDocumentStream->getData<uint8_t>());
                    bytesReaded += sizeof(uint8_t);
                } break;
                case 2: case 4: case 5: {
                    _characterDiapasonsModifyOperands.push_back(_wordDocumentStream->getData<uint16_t>());
                    bytesReaded += sizeof(uint16_t);
                } break;
                case 3: {
                    _characterDiapasonsModifyOperands.push_back(_wordDocumentStream->getData<uint32_t>());
                    bytesReaded += sizeof(uint32_t);
                } break;
                case 7: {
                    uint32_t operand = 0;
                    _wordDocumentStream->read((char*)&operand, 3);
                    _characterDiapasonsModifyOperands.push_back(operand);
                    bytesReaded += 3;
                } break;
                case 6: {
                    auto operandSize = _wordDocumentStream->getData<uint8_t>();
                    if (operandSize > sizeof(uint32_t))
                    {
                        std::cout << "Operand with type 6 has size > 4! Size: " << uint32_t(operandSize) << std::endl;
                        _characterDiapasonsModifyOperands.push_back(_wordDocumentStream->getData<uint32_t>());
                        _wordDocumentStream->ignore(operandSize - sizeof(uint32_t));
                    }
                    else
                    {
                        uint32_t operand = 0;
                        _wordDocumentStream->read((char*)&operand, operandSize);
                        _characterDiapasonsModifyOperands.push_back(operand);
                    }
                    bytesReaded += operandSize;
                } break;
                }
            }
        }
    }
}




void SDRL::makeContainers()
{
    modifyDiapasonsForWorkWithCharacterStream();
    readCharactersAndCreateContainers();
}

void SDRL::modifyDiapasonsForWorkWithCharacterStream()
{
    uint32_t characterPartIndex = 0;
    uint32_t subForCorrectingOffset = _charactersOffsets[characterPartIndex].fc.fc;

    uint32_t minOffset = 0;
    uint32_t maxOffset = minOffset
            + ((_charactersPositions.size() > 1)
                ? _charactersPositions[1] - _charactersPositions[0]
                : _charactersPositions[0])
            * 2;

    for (auto& offset : _characterDiapasons)
    {
        uint32_t newOffset = offset - subForCorrectingOffset;
        if (newOffset > maxOffset)
        {
            characterPartIndex++;
            if ( maxOffset != (_charactersOffsets[characterPartIndex].fc.fc - subForCorrectingOffset) )
            {
                subForCorrectingOffset += _charactersOffsets[characterPartIndex].fc.fc
                        - subForCorrectingOffset - maxOffset;
            }
            minOffset = maxOffset;
            maxOffset = minOffset
                    + (_charactersPositions[characterPartIndex + 1]
                    - _charactersPositions[characterPartIndex]) * 2;

            newOffset = offset - subForCorrectingOffset;
        }

        offset = newOffset;
    }
}

void SDRL::readCharactersAndCreateContainers()
{
    std::stack<std::shared_ptr<Container>> stackOfParentContainers;
    std::shared_ptr<Container> newElement;

    int counter_paragraph = 0;
    int counter_table = 0;

    _characterStream.seekg(0, _characterStream.beg);
    std::wstring str;
    for (int i = 0, size = _characterDiapasons.size() - 1; i < size; i++)
    {
        uint32_t stringSize = ((_characterDiapasons[i + 1] - _characterDiapasons[i]) / 2) - 1;
        str.resize(stringSize, 0);

        _characterStream.read((wchar_t*)str.data(), stringSize);
        uint32_t bytesReaded = _characterStream.gcount();

        wchar_t lastSymbol;
        _characterStream.read((wchar_t*)&lastSymbol, 1);

        if (bytesReaded > 0)
        {
            newElement = std::make_shared<TextContainer>(str);
        }

        std::wcout << L"Str: " << str << " | last symbol: " << (uint16_t)lastSymbol << std::endl;

        switch(lastSymbol)
        {
        case Mark_Paragraph: {
            counter_table = 0;
            counter_paragraph++;
        } break;
        case Mark_Table: {
            counter_paragraph = 0;
            counter_table++;
        } break;
        case Mark_Image: {} break;
        }
    }
}

