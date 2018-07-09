#ifndef CONTAINER_H
#define CONTAINER_H

#include <string>
#include <list>
#include <memory>

class Container
{
public:
    enum ContainerType {
        ContainerType_NoType = 0,
        ContainerType_Text,
        ContainerType_Table,
        ContainerType_TableLine,
        ContainerType_TableCell,
        ContainerType_Image,
        ContainerType_TypesAmount
    };

private:
    std::list<std::shared_ptr<Container>> _childContainers;

protected:
    ContainerType _type;

public:
    Container();
    virtual ~Container() = 0;

    inline void addChild(std::shared_ptr<Container>& container) { _childContainers.push_back(container); }
    inline const std::list<std::shared_ptr<Container>>& getChilds() { return _childContainers; }

    inline ContainerType getType() { return _type; }
};


#endif // CONTAINER_H
