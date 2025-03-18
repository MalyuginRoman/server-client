#include <string>
#include "scope.h"
#include "object.h"

class scopeP
{
public:
    std::map<std::string, std::function<ICommand*()>> *m_map;
    std::map<std::string, std::string> *m_scope;

    scopeP(std::map<std::string, std::function<ICommand*()>> *m_map,
           std::map<std::string, std::string> *m_scope) :
        m_map(m_map),
        m_scope(m_scope)
    {
    }
};

scope::scope(std::map<std::string, std::function<ICommand*()>> *m_map,
             std::map<std::string, std::string> *m_scope) :
    imp(new scopeP(m_map, m_scope))
{
}

scope::~scope() { delete imp;}

void scope::resolve(std::string key_s, std::string key_f, std::function<ICommand*()> func)
{
    imp->m_scope->emplace(key_s, key_f);
    imp->m_map->emplace(key_f, func);
}
