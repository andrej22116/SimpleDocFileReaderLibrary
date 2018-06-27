#ifndef TEXTCONTAINER_H
#define TEXTCONTAINER_H

#include "ClassContainer/container.h"

class TextContainer : public Container
{
private:
    std::string _text;
    //std::string _formatedText;

public:
    TextContainer(std::string text);
    virtual ~TextContainer() override;

    inline std::string getText() { return _text; }
    inline const std::string& watchText() { return _text; }
    //inline const std::string& getFormatedText() { return _formatedText; }
};

#endif // TEXTCONTAINER_H
