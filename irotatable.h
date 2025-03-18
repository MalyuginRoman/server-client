#pragma once
#include "object.h"

class IRotatable
{
public:
    virtual bool getAngular(object *obj, int dt) = 0;
    virtual bool setAngular(object *obj) = 0;
    virtual bool getAngularVelocity(object *obj, int dv) = 0;
    virtual bool setAngularVelocity(object *obj) = 0;
};
