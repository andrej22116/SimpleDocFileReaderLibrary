#ifndef WORDBINARYFILEFORMATREADER_H
#define WORDBINARYFILEFORMATREADER_H

#include "ClassWindowsCompoundBinaryFileFormatReader/windowscompoundbinaryfileformatreader.h"
#include "ClassTextContainer/textcontainer.h"

class WordBinaryFileFormatReader : protected WindowsCompoundBinaryFileFormatReader
{
private:
    std::istream* _wordDocumentStream;
    std::istream* _tableStream;

    std::vector<TextContainer> _textContainers;

public:
    WordBinaryFileFormatReader(const std::string& fileName);
    WordBinaryFileFormatReader(std::istream& stream);

    inline const std::vector<TextContainer>& getContainers();

private:
    void readDocument();

    std::istream& getStream(std::string streamName);
};

#endif // WORDBINARYFILEFORMATREADER_H
