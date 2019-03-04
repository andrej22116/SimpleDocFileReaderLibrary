#include <SDRL/Containers/textcontainer.h>//"textcontainer.h"
/*
TextContainer::TextContainer(const std::wstring& text)
{
    _text = text;
}
*/
TextContainer::TextContainer(std::wstring text)
{
    _type = ContainerType_Text;
    _text = text;
}

TextContainer::~TextContainer()
{

}
