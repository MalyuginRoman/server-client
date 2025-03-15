#include <map>
#include <list>
#include <iostream>
#include "object.h"
#include "objposition.h"

double object_position::local_ext(double f1, double f2, int num, int count)
{
    return (f1 + num * f2/count);
}

bool object_position::isXY(std::map<int, system_okr> *p_map, objectVector *vector, int i, int j)
{
    bool isX = ((p_map->at(j).XY.Xmin - vector->at(i)->place().placeX) < eps) &&
               ((vector->at(i)->place().placeX - p_map->at(j).XY.Xmax) < eps);
    bool isY = ((p_map->at(j).XY.Ymin - vector->at(i)->place().placeY) < eps) &&
               ((vector->at(i)->place().placeY - p_map->at(j).XY.Ymax) < eps);
    return (isX && isY);
}

bool object_position::isXY_r(std::map<int, system_okr> *p_map, object *obj, int j)
{
    bool isX = ((p_map->at(j).XY.Xmin - obj->place().placeX) < eps) &&
               ((obj->place().placeX - p_map->at(j).XY.Xmax) < eps);
    bool isY = ((p_map->at(j).XY.Ymin - obj->place().placeY) < eps) &&
               ((obj->place().placeY - p_map->at(j).XY.Ymax) < eps);
    return (isX && isY);
}

std::map<int, system_okr> object_position::func_name(int number)  // формируем системы окрестностей
{
    double Xstart = 0.;
    double Ystart = 0.;
    int sizeXY = 0;
    std::map<int, system_okr> p_map_c_a;
    int count_system;
    if(number == 1)
        count_system = count_system_1;
    else
        count_system = count_system_2;

    for(int i = 0; i < count_system; i++)
        for(int j = 0; j < count_system; j++)
        {
            system_okr a1;
            a1.XY.Xmin = local_ext(Xstart, Xmax, i, count_system);
            a1.XY.Xmax = local_ext(Xstart, Xmax, i + 1, count_system);
            a1.XY.Ymin = local_ext(Ystart, Ymax, j, count_system);
            a1.XY.Ymax = local_ext(Ystart, Ymax, j + 1, count_system);
            a1.id_syst_okr = sizeXY;
            a1.isEmpty = true;
            a1.countObject = 0;
            p_map_c_a.emplace(sizeXY, a1);
            sizeXY++;
        }
return p_map_c_a;
}

std::map<int, system_okr> *object_position::func_obj(std::map<int, system_okr> *p_map,
                                                    objectVector *vector)   // помещаем объекты в системы окресностей
{
    size_t countObj = vector->count();
    std::map<int, system_okr> *p_map_c = p_map;
    for(int i = 0; i < countObj; i++)
    {
        size_t count1 = p_map_c->size();
        for(int j = 0; j < count1; j++)
        {
            if(isXY(p_map_c, vector, i, j))
            {
                bool isWrite = false;
                int id_obj = vector->at(i)->objectID();
                for(std::vector<int>::iterator it = p_map_c->at(j).obj_id.begin();
                                               it != p_map_c->at(j).obj_id.end();)
                    if(*it == id_obj)
                    {
                        isWrite = true;
                        ++it;
                        break;
                    }
                    else
                        ++it;
                if(!isWrite)
                {
                    p_map_c->at(j).countObject++;
                    p_map_c->at(j).isEmpty = false;
                    if(!p_map_c->at(j).obj_id.empty())
                    {
                        int count2 = p_map_c->at(j).obj_id.size();
                        for(int k = 0; k < count2; k++)
                        {
                            if(p_map_c->at(j).obj_id.at(k) == id_obj)
                                break;
                        }
                    }
                    p_map_c->at(j).obj_id.push_back(id_obj);
                    break;
                }
                if(isWrite)
                    break;
            }
        }
    }
    return p_map_c;
}

std::map<int, system_okr> *object_position::func_obj_r(std::map<int, system_okr> *p_map,
                                                    object *obj)   // помещаем rocket в системы окресностей
{
    std::map<int, system_okr> *p_map_c = p_map;
    size_t count1 = p_map_c->size();
    for(int j = 0; j < count1; j++)
    {
        if(isXY_r(p_map_c, obj, j))
        {
            bool isWrite = false;
            int id_obj =obj->objectID();
            for(std::vector<int>::iterator it = p_map_c->at(j).obj_id.begin();
                                           it != p_map_c->at(j).obj_id.end();)
                if(*it == id_obj)
                {
                    isWrite = true;
                    ++it;
                    break;
                }
                else
                    ++it;
            if(!isWrite)
            {
                p_map_c->at(j).countObject++;
                p_map_c->at(j).isEmpty = false;
                if(!p_map_c->at(j).obj_id.empty())
                {
                    int count2 = p_map_c->at(j).obj_id.size();
                    for(int k = 0; k < count2; k++)
                    {
                        if(p_map_c->at(j).obj_id.at(k) == id_obj)
                            break;
                    }
                }
                p_map_c->at(j).obj_id.push_back(id_obj);
                break;
            }
            if(isWrite)
                break;
        }
    }
    return p_map_c;
}
