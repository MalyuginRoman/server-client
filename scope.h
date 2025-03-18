#pragma once
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
private:
    class scopeP* imp;
};
