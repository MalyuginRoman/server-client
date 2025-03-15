#ifndef ORDER_H
#define ORDER_H
#include <string>
#include <vector>
#include <list>

class order
{
public:
    order(int playerID, int objectID, std::string actionName, std::string specParam);
    ~order();

    int playerID() const;
    int objectID() const;
    std::string actionName() const;
    std::string specParam() const;

    void setObjectID(int value);
    void setActionName(std::string value);
    void setSpecParam(std::string value);

private:
    class orderP* imp;
};

class orderVector
{
public:
    orderVector();
    ~orderVector();
    void reset();

    order* add(int playerID, int objectID, std::string actionName, std::string specParam);
    bool isEmpty() const;
    size_t count() const;
    const std::list/*vector*/<order *> vector() const;
    //order* at(int i);
private:
    class orderVectorP* imp;
};

#endif // ORDER_H
