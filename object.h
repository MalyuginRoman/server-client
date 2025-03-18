#pragma once
#include "minor.h"
#include "imoving.h"
#include "irotatable.h"

class object : public IMoving, IRotatable
{
public:
    object(int playerID, int objectID, react state, coord place);
    ~object();

    int playerID() const;
    int objectID() const;
    react state() const;
    coord place() const;

    void setState(react state);
    void setPlace(coord place);

    bool getPosition(object *obj, int dt);
    bool setPosition(object *obj);
    bool getVelocity(object *obj, double du);
    bool setVelocity(object *obj);

    bool getAngular(object *obj, int dt);
    bool setAngular(object *obj);
    bool getAngularVelocity(object *obj, int dv);
    bool setAngularVelocity(object *obj);

    bool getFuel(object *obj/*, int dF*/);

private:
    class objectP* imp;
};

class objectVector
{
public:
    objectVector();
    ~objectVector();
    void reset();

    object* add(int playerID, int objectID, react state, coord place);
    void del(int i);
    bool isEmpty() const;
    size_t count() const;
    const std::vector<object *> vector() const;
    object* at(int i);
private:
    class objectVectorP* imp;
};
