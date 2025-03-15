#ifndef OBJPOSITION_H
#define OBJPOSITION_H
#include <map>
#include <list>
#include "object.h"

class object_position
{
public:
    double local_ext(double f1, double f2, int num, int count);
    bool isXY(std::map<int, system_okr> *p_map, objectVector *vector, int i, int j);
    bool isXY_r(std::map<int, system_okr> *p_map, object *obj, int j);
    std::map<int, system_okr> func_name(int number);
    std::map<int, system_okr> *func_obj(std::map<int, system_okr> *p_map, objectVector *vector);
    std::map<int, system_okr> *func_obj_r(std::map<int, system_okr> *p_map, object *obj);
};

#endif // OBJPOSITION_H
