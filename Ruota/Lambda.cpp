#include "Ruota.h"

Lambda::Lambda(Scope *parent, Node main, std::vector<DataType> params, std::vector<std::string> keys, DataType return_type)
{
    this->parent = parent;
    this->main = main;
    this->params = params;
    this->keys = keys;
    this->return_type = return_type;
}

DataPoint Lambda::evaluate(std::vector<DataPoint> args) const
{
    Scope ns(parent);
    for (int i = 0; i < args.size(); i++)
    {
        ns.declareVariable(keys[i], params[i]).set(args[i]);
    }

    if (!return_type.equals("null"))
    {
        DataPoint r(return_type);
        auto raw = main.evaluate(&ns);
        if (raw.flag_pointer)
        {
            raw.flag_return = false;
            raw.flag_pointer = false;
            return raw;
        }
        else
        {
            r.set(raw);
            return r;
        }
    }
    else
    {
        main.evaluate(&ns);
        return DataPoint();
    }
}

Lambda Lambda::clone(Scope *new_parent) const
{
    return Lambda(new_parent, main, params, keys, return_type);
}