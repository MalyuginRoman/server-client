#pragma once
class object;

class IMoving
{
public:
    virtual bool getPosition(object *obj, int dt) = 0;
    virtual bool setPosition(object *obj) = 0;
    virtual bool getVelocity(object *obj, double du) = 0;
    virtual bool setVelocity(object *obj) = 0;
};
