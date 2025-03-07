#include "order.h"

class orderP
{
public:
    int playerID;
    int objectID;
    std::string actionName;
    std::string specParam;

    orderP(int playerID, int objectID, std::string actionName, std::string specParam) :
        playerID(playerID),
        objectID(objectID),
        actionName(actionName),
        specParam(specParam)
    { }
    ~orderP()
    { }
};

order::order(int playerID, int objectID, std::string actionName, std::string specParam) :
    imp(new orderP(playerID, objectID, actionName, specParam))
{
}

order::~order()
{
    delete imp;
}

int order::playerID() const
{
    return imp->playerID;
}

int order::objectID() const
{
    return imp->objectID;
}

std::string order::actionName() const
{
    return imp->actionName;
}

std::string order::specParam() const
{
    return imp->specParam;
}

void order::setObjectID(int value)
{
    imp->objectID = value;
}

void order::setActionName(std::string value)
{
    imp->actionName = value;
}

//****************************** orderVector ***************************
class orderVectorP
{
public:
    std::vector<order*> vector;

    ~orderVectorP()
    {
       vector.clear();
    }
    void reset()
    {
        vector.clear();
    }
    order *add(int playerID, int objectID, std::string actionName, std::string specParam)
    {
        order* playerOrder = new order(playerID, objectID, actionName, specParam);
        vector.push_back(playerOrder);
        return playerOrder;
    }
    order *at(int i)
    {
        return vector.at(i);
    }
};
orderVector::orderVector() : imp(new orderVectorP)
{
}
orderVector::~orderVector()
{
    delete imp;
}
void orderVector::reset()
{
    imp->reset();
}
order *orderVector::add(int playerID, int objectID, std::string actionName, std::string specParam)
{
    return imp->add(playerID, objectID, actionName, specParam);
}
bool orderVector::isEmpty() const
{
    return imp->vector.empty();
}
size_t orderVector::count() const
{
    return imp->vector.size();
}
const std::vector<order *> &orderVector::vector() const
{
    return imp->vector;
}

order *orderVector::at(int i)
{
    return imp->at(i);
}
