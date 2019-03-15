#include "Ruota.h"

std::unordered_map<std::string, std::vector<DataPoint> (*)(std::vector<DataPoint>)> RuotaWrapper::registers = {};

void RuotaWrapper::register_command(std::string key, std::vector<DataPoint> (*value)(std::vector<DataPoint>))
{
    registers[key] = value;
}

Node::Node()
{
    this->nt = NULL_NODE;
}

Node::Node(DataPoint dp)
{
    this->dp = dp;
    this->nt = VALUE;
}

Node::Node(NodeType nt, std::vector<Node> params)
{
    this->nt = nt;
    this->params = params;
}

Node::Node(NodeType nt, std::vector<std::string> values)
{
    this->nt = nt;
    this->values = values;
}

Node::Node(const Node &n)
{
    this->nt = n.nt;
    this->params = n.params;
    this->values = n.values;
    this->dp = n.dp;
}

DataPoint Node::evaluate(Scope *scope) const
{
    switch (nt)
    {
    case SEQUENCE_LAX:
    {
        std::vector<DataPoint> seq;
        for (auto &p : params)
        {
            DataPoint dp = p.evaluate(scope);
            if (dp.flag_return)
                return dp;
            if (p.nt == B_RANGE)
            {
                for (auto &d : dp.getVector())
                    seq.push_back(d);
            }
            else
                seq.push_back(dp);
        }
        auto r = DataPoint(seq);
        r.dt.is_array = true;
        if (r.value_vector.size() == 1)
            return r.value_vector[0];
        return r;
    }
    case SEQUENCE:
    {
        std::vector<DataPoint> seq;
        for (auto &p : params)
        {
            DataPoint dp = p.evaluate(scope);
            if (dp.flag_return)
                return dp;
            if (p.nt == B_RANGE)
            {
                for (auto &d : dp.getVector())
                    seq.push_back(d);
            }
            else
                seq.push_back(dp);
        }
        auto r = DataPoint(seq);
        r.dt.is_array = true;
        return r;
        //r.dt.arr_dimensions.push_back(seq.size());
    }
    case VALUE:
        return dp;
    case VARIABLE:
        return scope->getVariable(values[0]);
    case SCOPE:
    {
        Scope sv(scope);
        DataPoint raw = params[0].evaluate(&sv);
        if (raw.flag_pointer && raw.flag_return)
            return raw;
        else if (raw.flag_return)
        {
            DataPoint r;
            r = DataPoint(raw.getType());
            r.set(raw);
            r.flag_return = true;
            return r;
        }
        return DataPoint();
    }
    case FLAG_POINTER:
    {
        DataPoint a_ = params[0].evaluate(scope);
        a_.flag_pointer = true;
        return a_;
    }
    case RETURN:
    {
        DataPoint a_ = params[0].evaluate(scope);
        a_.flag_return = true;
        return a_;
    }
    case B_ADD:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        return a_.add(b_);
    }
    case B_SUB:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        return a_.sub(b_);
    }
    case B_MUL:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        return a_.mul(b_);
    }
    case B_DIV:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        return a_.div(b_);
    }
    case B_MOD:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        return a_.mod(b_);
    }
    case B_POW:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        return a_.pow(b_);
    }
    case B_LESS:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        return a_.less(b_);
    }
    case B_MORE:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        return a_.more(b_);
    }
    case B_ELESS:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        return a_.eless(b_);
    }
    case B_EMORE:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        return a_.emore(b_);
    }
    case B_EQUALS:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        return a_.equals(b_);
    }
    case B_NEQUALS:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        return a_.nequals(b_);
    }
    case B_OR:
    {
        for (auto &p : params)
        {
            auto dp = p.evaluate(scope);
            if (dp.getBool())
                return DataPoint(true);
        }
        return DataPoint(false);
    }
    case B_AND:
    {
        for (auto &p : params)
        {
            auto dp = p.evaluate(scope);
            if (!dp.getBool())
                return DataPoint(false);
        }
        return DataPoint(true);
    }
    case B_ITER:
    {
        /*
        ITER
            [GENERIC
                [TEMP type]
                [gendata]
            ] 
            {...}
        */
        Scope *object_scope = new Scope(scope);
        scope->declareVariable(params[0].params[0].values[0], DataType({params[1].params[0]}, object_scope, params[0].params[0].values[0]));
        for (auto &p : params[0].params[1].params)
        {
            object_scope->generics.push_back({p.values[0], DataType("UNDEF")});
        }
        params[1].params[0].evaluate(object_scope);
        auto r = scope->getVariable(params[0].params[0].values[0]);
        return r;
    }
    case B_DEC:
    {
        DataType ndt;
        if (params[0].nt == VARIABLE)
        {
            ndt = DataType(params[0].values[0]);
            if (!ndt.isBaseNumber())
            {
                Scope *current = scope;
                while (current != NULL)
                {
                    bool flag = false;
                    for (auto &g : current->generics)
                    {
                        if (g.first == params[0].values[0])
                        {
                            flag = true;
                        }
                    }
                    if (flag)
                    {
                        break;
                    }
                    current = current->parent;
                }
                if (current != NULL)
                {
                    for (auto &g : current->generics)
                    {
                        if (g.first == params[0].values[0])
                        {
                            ndt = g.second;
                        }
                    }
                }
                else
                    ndt = DataType(
                        scope->getVariable(params[0].values[0]).getType().prototype,
                        scope->getVariable(params[0].values[0]).getType().getProtoObject(),
                        params[0].values[0]);
            }
        }
        else if (params[0].nt == B_GENERIC)
        {
            std::vector<DataType> qualifiers;
            for (auto &n : params[0].params[1].params)
            {
                Node temp(B_DEC, {n});
                temp.values.push_back("$");
                qualifiers.push_back(temp.evaluate(scope).getType());
            }
            Node temp(B_DEC, {params[0].params[0]});
            temp.values.push_back("$");
            DataType baseType = temp.evaluate(scope).getType();

            baseType.qualifiers = qualifiers;
            ndt = baseType;
        }
        else if (params[0].nt == B_INDEX)
        {
            std::vector<unsigned long long> dimensions;
            auto evals = params[0].params[1].evaluate(scope).value_vector;
            for (auto &e : evals)
            {
                dimensions.push_back(e.getLong());
            }
            Node temp(B_DEC, {params[0].params[0]});
            temp.values.push_back("$");
            DataType baseType = temp.evaluate(scope).getType();

            baseType.is_array = true;
            baseType.arr_dimensions = dimensions;
            ndt = baseType;
        }
        else if (params[0].nt == B_EXEC)
        {
            std::vector<DataType> args;
            auto evals = params[0].params[1].params;
            Scope temp_s(scope);
            for (auto &e : evals)
            {
                args.push_back(e.evaluate(&temp_s).getType());
            }
            Node temp(B_DEC, {params[0].params[0]});
            temp.values.push_back("$");
            auto retTypes = {temp.evaluate(scope).getType()};
            ndt = DataType(retTypes, args);
        }
        else
        {
            Node current = params[0];
            while (current.nt == B_OBJ_INDEX)
                current = current.params[1];
            ndt = DataType(params[0].evaluate(scope).getType().prototype, params[0].evaluate(scope).getType().getProtoObject(), current.values[0]);
        }
        return scope->declareVariable(values[0], ndt);
    }
    case B_SET:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        a_.set(b_);
        return a_;
    }
    case B_RANGE:
    {
        DataPoint a_ = params[1].params[0].evaluate(scope);
        DataPoint b_ = params[1].params[1].evaluate(scope);
        DataPoint step = DataPoint((long)1);
        if (params[1].params.size() > 2)
            step = params[1].params[2].evaluate(scope);
        Node c(B_DEC, {params[0]});
        c.values.push_back("$");
        DataPoint c_ = c.evaluate(scope);
        c_.set(a_);
        std::vector<DataPoint> seq;
        while (c_.less(b_).getBool())
        {
            DataPoint dp(c_.getType());
            dp.set(c_);
            seq.push_back(dp);
            c_.set(c_.add(step));
        }
        auto r = DataPoint(seq);
        r.dt.is_array = true;
        return r;
    }
    case B_EXEC:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        std::vector<DataType> types;
        for (auto &t : b_.value_vector)
            types.push_back(t.getType());
        auto lambda = a_.getLambda(types);
        if (lambda.parent == NULL)
            throw std::runtime_error("Error: Lambda not found.");
        return lambda.evaluate(b_.value_vector);
    }
    case U_NEW:
    {
        if (params[0].nt == B_EXEC)
        {
            Node a(B_DEC, {params[0].params[0]});
            a.values.push_back("$");
            auto a_ = a.evaluate(scope);
            DataPoint r(a_.getType());
            Scope *object_scope = new Scope(a_.getType().getProtoObject()->parent);
            for (int i = 0; i < a_.getType().qualifiers.size(); i++)
            {
                object_scope->generics.push_back({a_.getType().getProtoObject()->generics[i].first, a_.getType().qualifiers[i]});
            }
            r.setObject(object_scope);
            r.getObject()->declareVariable("self", r.getType());
            r.getObject()->variables["self"].set(r);
            a_.getType().prototype.back().evaluate(object_scope);
            auto args = params[0].params[1].evaluate(scope).value_vector;
            std::vector<DataType> params;
            for (auto &arg : args)
                params.push_back(arg.getType());
            auto lambda = r.getObject()->getVariable("init").getLambda(params);
            if (lambda.parent == NULL)
                throw std::runtime_error("Error: Init lambda not found!");
            lambda.evaluate(args);
            return r;
        }
        else
        {
            Node a(B_DEC, {params[0]});
            a.values.push_back("$");
            auto a_ = a.evaluate(scope);
            DataPoint r(a_.getType());
            Scope *object_scope = new Scope(a_.getType().getProtoObject()->parent);
            for (int i = 0; i < a_.getType().qualifiers.size(); i++)
            {
                object_scope->generics.push_back({a_.getType().getProtoObject()->generics[i].first, a_.getType().qualifiers[i]});
            }
            r.setObject(object_scope);
            r.getObject()->declareVariable("self", r.getType());
            r.getObject()->variables["self"].set(r);
            a_.getType().prototype.back().evaluate(object_scope);
            return r;
        }
    }
    case B_STATIC:
    {
        Scope *object_scope = new Scope(scope);
        object_scope->stat = true;
        scope->declareVariable(values[0], DataType({params[0].params[0]}, object_scope, values[0]));
        params[0].params[0].evaluate(object_scope);
        auto r = scope->getVariable(values[0]);
        return r;
    }
    case B_OBJ:
    {
        Scope *object_scope = new Scope(scope);
        scope->declareVariable(values[0], DataType({params[0].params[0]}, object_scope, values[0]));
        params[0].params[0].evaluate(object_scope);
        auto r = scope->getVariable(values[0]);
        return r;
    }
    case B_OBJ_INDEX:
    {
        DataPoint a_ = params[0].evaluate(scope);
        if (a_.getObject() == NULL)
            throw std::runtime_error("No object found to be indexed!");
        return params[1].evaluate(a_.getObject());
    }
    case LAM_DEC:
    {
        Node a(params[0]);
        a.values.clear();
        a.values.push_back("$");
        DataPoint a_ = a.evaluate(scope);
        std::vector<std::string> keys;
        for (auto &p : a.params[0].params[1].params)
        {
            keys.push_back(p.values[0]);
        }
        auto retType = a_.getType().return_type[0];
        Lambda l(scope, params[1], a_.getType().params, keys, retType);
        scope->variables[params[0].values[0]].dt = a_.getType();
        auto r = DataPoint(&scope->variables[params[0].values[0]]);
        r.addLambda(l);
        return r;
    }
    case B_DO_LOOP:
    {
        if (params[0].nt == B_ITER)
        {
            Scope ns(scope);
            auto iter = params[0].params[0].evaluate(&ns);
            auto val_arr = params[0].params[1].evaluate(scope).getVector();
            for (auto &v : val_arr)
            {
                iter.set(v);
                auto r = params[1].evaluate(&ns);
                if (r.flag_return && !r.getType().equals("break"))
                    return r;
                if (r.getType().equals("break"))
                    break;
            }
        }
        else
        {
            Scope ns(scope);
            auto a = params[0];
            while (a.evaluate(scope).getBool())
            {
                auto r = params[1].evaluate(&ns);
                if (r.flag_return && !r.getType().equals("break"))
                    return r;
                if (r.getType().equals("break"))
                    break;
            }
        }
        return DataPoint();
    }
    case B_IF_ELSE:
    {
        DataPoint a_ = params[0].evaluate(scope);
        if (a_.getBool())
        {
            return params[1].evaluate(scope);
        }
        else if (params.size() > 2)
        {
            return params[2].evaluate(scope);
        }
        return DataPoint();
    }
    case B_INDEX:
    {
        DataPoint a_ = params[0].evaluate(scope);
        DataPoint b_ = params[1].evaluate(scope);
        return a_.index(b_.getVector());
    }
    case U_LEN:
    {
        DataPoint a_ = params[0].evaluate(scope);
        return DataPoint((long)a_.getVector().size());
    }
    case U_IS:
    {
        DataPoint a_ = params[0].evaluate(scope);
        if (a_.dpn == VAR_REF)
        {
            return DataPoint((bool)(a_.value_ref->value_object != NULL));
        }
        else
        {
            return DataPoint((bool)(a_.value_object != NULL));
        }
    }
    case U_REM:
    {
        DataPoint a_ = params[0].evaluate(scope);
        a_.setObject(NULL);
        return a_;
    }
    case B_REG:
    {
        DataPoint a_ = params[0].evaluate(scope);
        auto r = DataPoint(RuotaWrapper::registers[values[0]](a_.getVector()));
        r.dt.is_array = true;
        return r;
    }
    }
}