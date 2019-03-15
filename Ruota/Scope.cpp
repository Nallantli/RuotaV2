#include "Ruota.h"

Scope::Scope(Scope *parent)
{
    this->parent = parent;
}

DataPoint Scope::getVariable(const std::string &key)
{
    Scope *current = this;
    while (current->variables.find(key) == current->variables.end())
    {
        current = current->parent;
    }
    if (current == NULL)
    {
        throw std::runtime_error("Cannot find variable " + key);
    }
    return DataPoint(&current->variables[key]);
}

DataPoint Scope::declareVariable(const std::string &key, DataType dt)
{
    this->variables[key] = DataPoint(dt);
    return DataPoint(&this->variables[key]);
}

const std::string Scope::getDebug() const
{
    std::string s = "VARIABLES--------------\n";
    for (auto &dp : variables)
    {
        s += dp.first + "\t" + dp.second.getDebug() + "\n";
    }
    s += "GENERICS--------------\n";
    for (auto &dp : generics)
    {
        s += dp.first + "\t" + dp.second.getString() + "\n";
    }
    s += "-----------------------\n";
    return s;
}

Scope *Scope::clone() const
{
    Scope *ns = new Scope(parent);
    ns->stat = this->stat;
    ns->generics = this->generics;
    for (auto &v : variables)
    {
        ns->variables[v.first] = v.second.clone(ns);
    }
    return ns;
}