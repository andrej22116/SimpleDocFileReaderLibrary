#include "wordbinaryfileformatreader.h"
#include "ClassInputBinaryStream/inputbinarystream.h"

#include "wbff_structures.h"

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
    // Get streams
    auto& wordDocumentStream = getStream("WordDocument");
    InputBinaryStream binWordStream(wordDocumentStream);

    auto fibBase = binWordStream.getData<FIB_Base>();
    std::string nameofTableStream("0Table");
    if (fibBase.G_fWhichTblStm == 1)
    {
        nameofTableStream = "1Table";
    }

    auto& tableStream = getStream(nameofTableStream);
    InputBinaryStream binTableStream(tableStream);

    std::cout << "FIB2007 size: " << sizeof(FIB_RgFcLcb2007) << std::endl;
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
