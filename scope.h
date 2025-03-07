#ifndef SCOPE_H
#define SCOPE_H
#include <iostream>
#include <list>
#include <map>
#include <functional>
#include "icommand.h"

class scope
{
public:
    scope(std::map<std::string, std::function<ICommand*()>> *m_map,
          std::map<std::string, std::string> *m_scope);
    ~scope();
    std::map<std::string, std::function<ICommand*()>>* m_map;
    std::map<std::string, std::string>* m_scope;
    void resolve(std::string key_s, std::string key_f, std::function<ICommand*()> func);
    scope* fillin(scope *scope_old, std::map<int, system_okr> p_map_a,
                 std::map<int, system_okr> p_map_b, objectVector *vector);
private:
    class scopeP* imp;
};

#endif // SCOPE_H
