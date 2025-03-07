#include <map>
#include <list>
#include "object.h"
#include "objposition.h"

double object_position::local_ext(double f1, double f2, int num, int count)
{
    return (f1 + num * f2/count);
}

bool object_position::isXY(std::map<int, system_okr> p_map, objectVector *vector, int i, int j)
{
    bool isX = ((p_map.at(j).XY.Xmin - vector->at(i)->place().placeX) < eps) &&
               ((vector->at(i)->place().placeX - p_map.at(j).XY.Xmax) < eps);
    bool isY = ((p_map.at(j).XY.Ymin - vector->at(i)->place().placeY) < eps) &&
               ((vector->at(i)->place().placeY - p_map.at(j).XY.Ymax) < eps);
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

std::map<int, system_okr> object_position::func_obj(std::map<int, system_okr> p_map, objectVector *vector)   // помещаем объекты в системы окресностей
{
    size_t countObj = vector->count();
    std::map<int, system_okr> p_map_c = p_map;
    for(int i = 0; i < countObj; i++)
    {
        size_t count1 = p_map_c.size();
        for(int j = 0; j < count1; ++j)
        {
            if(isXY(p_map_c, vector, i, j))
            {
                p_map_c.at(j).countObject++;
                p_map_c.at(j).isEmpty = false;
                int id_obj = vector->at(i)->objectID();
                p_map_c.at(j).obj_id.push_back(id_obj);
                break;
            }
        }
    }
    return p_map_c;
}
