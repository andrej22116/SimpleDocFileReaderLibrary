#ifndef TEXTCONTAINER_H
#define TEXTCONTAINER_H

#include "Containers/ClassContainer/container.h"

class TextContainer : public Container
{
private:
    std::wstring _text;
    //std::string _formatedText;

public:
    //TextContainer(const std::wstring& text);
    TextContainer(std::wstring text);
    virtual ~TextContainer() override;

    inline std::wstring getText() { return _text; }
    inline const std::wstring& watchText() { return _text; }
    //inline const std::string& getFormatedText() { return _formatedText; }
};

#endif // TEXTCONTAINER_H
