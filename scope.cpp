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

scope *scope::fillin(scope *scope_old, std::map<int, system_okr> p_map_a, std::map<int, system_okr> p_map_b, objectVector *vector)
{
    scope *scope_new = scope_old;

    scope_new->resolve("Scope1",
                    "MoveCommand",
                    [&p_map_a, &p_map_b, &vector]() {
                    return new MoveCommand(&p_map_a, &p_map_b, vector->at(0)); });
    scope_new->resolve("Scope1",
                    "RotateCommand",
                    [&vector]() { return new RotateCommand(vector->at(0)); });
    scope_new->resolve("Scope1",
                    "LogerCommand",
                    []() { return new LogerCommand(); });
    scope_new->resolve("Scope1",
                    "EmptyCommand",
                    []() { return new EmptyCommand(); });
    scope_new->resolve("Scope1",
                    "HardStopCommand",
                    []() { return new HardStopCommand(); });
    scope_new->resolve("Scope1",
                    "SoftStopCommand",
                    []() { return new SoftStopCommand(); });
    scope_new->resolve("Scope1",
                    "StartMotion",
                    [&vector]() { return new StartMotion(vector->at(0), vector->at(0)->state().velocity); });
    scope_new->resolve("Scope1",
                    "StopMotion",
                    [&vector]() { return new StopMotion(vector->at(0)); });

    return scope_new;
}
