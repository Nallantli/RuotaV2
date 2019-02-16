#include "Ruota.h"

DataPoint::DataPoint()
{
    //this->dpt = UNDEFINED;
    this->dpn = END_NODE;
}

DataPoint::DataPoint(void *POINTER)
{
    this->POINTER = POINTER;
    this->dpn = END_NODE;
    this->dt = DataType("void");
}

DataPoint::DataPoint(const DataPoint &dp)
{
    this->flag_return = dp.flag_return;
    this->flag_pointer = dp.flag_pointer;
    if (dp.dpn == VAR_REF)
    {
        this->dpn = VAR_REF;
        this->value_ref = dp.value_ref;
    }
    else
    {
        this->dt = dp.dt;
        this->dpn = END_NODE;
        if (dp.value_object != NULL)
            this->setObject(dp.value_object);
        if (dt.equals("int"))
            this->value_int = dp.value_int;
        if (dt.equals("long"))
            this->value_long = dp.value_long;
        if (dt.equals("double"))
            this->value_double = dp.value_double;
        if (dt.equals("long"))
            this->value_long = dp.value_long;
        if (dt.equals("float"))
            this->value_float = dp.value_float;
        if (dt.equals("bool"))
            this->value_boolean = dp.value_boolean;
        if (dt.equals("short"))
            this->value_short = dp.value_short;
        if (dt.equals("char"))
            this->value_char = dp.value_char;
        if (dt.equals("void"))
            this->POINTER = dp.POINTER;
        if (dt.is_array)
        {
            for (int i = 0; i < dp.value_vector.size(); i++)
                value_vector.push_back(DataPoint(dp.value_vector[i]));
        }
        if (dt.is_lambda)
        {
            this->poly_lambda = dp.poly_lambda;
        }
    }
}
DataPoint::DataPoint(DataPoint *dp)
{
    this->value_ref = dp;
    while (this->value_ref->dpn == VAR_REF)
        this->value_ref = this->value_ref->value_ref;
    this->dpn = VAR_REF;
}

DataPoint::DataPoint(std::vector<DataPoint> value_vector)
{
    this->dpn = END_NODE;
    this->value_vector = value_vector;
}

DataPoint::DataPoint(DataType dt)
{
    this->dt = dt;
    this->dpn = END_NODE;
    if (dt.equals("void"))
        this->POINTER = NULL;
    if (dt.is_array)
    {
        for (int i = 0; i < dt.arr_dimensions[0]; i++)
            value_vector.push_back(DataPoint(dt.getSubType()));
    }
}

DataPoint::DataPoint(int value)
{
    this->dt = DataType("int");
    this->dpn = END_NODE;
    this->value_int = value;
}

DataPoint::DataPoint(long value)
{
    this->dt = DataType("long");
    this->dpn = END_NODE;
    this->value_long = value;
}

DataPoint::DataPoint(short value)
{
    this->dt = DataType("short");
    this->dpn = END_NODE;
    this->value_short = value;
}

DataPoint::DataPoint(char value)
{
    this->dt = DataType("char");
    this->dpn = END_NODE;
    this->value_char = value;
}

DataPoint::DataPoint(bool value)
{
    this->dt = DataType("bool");
    this->dpn = END_NODE;
    this->value_boolean = value;
}

DataPoint::DataPoint(double value)
{
    this->dt = DataType("double");
    this->dpn = END_NODE;
    this->value_double = value;
}

DataPoint::DataPoint(float value)
{
    this->dt = DataType("float");
    this->dpn = END_NODE;
    this->value_float = value;
}

DataPoint DataPoint::add(const DataPoint &dp) const
{
    if (dpn == VAR_REF)
        return value_ref->add(dp);

    DataPoint res;
    res.dt = this->dt;

    if (dt.equals("int"))
        res.value_int = this->getInt() + dp.getInt();
    else if (dt.equals("long"))
        res.value_long = this->getLong() + dp.getLong();
    else if (dt.equals("short"))
        res.value_short = this->getShort() + dp.getShort();
    else if (dt.equals("char"))
        res.value_char = this->getChar() + dp.getChar();
    else if (dt.equals("bool"))
        res.value_boolean = this->getBool() + dp.getBool();
    else if (dt.equals("double"))
        res.value_double = this->getDouble() + dp.getDouble();
    else if (dt.equals("float"))
        res.value_float = this->getFloat() + dp.getFloat();
    else if (dt.is_array)
    {
        std::vector<DataPoint> elements;
        for (auto &e : this->getVector())
            elements.push_back(e);
        for (auto &e : dp.getVector())
            elements.push_back(e);
        DataPoint temp(elements);
        temp.dt.is_array = true;
        res.set(temp);
    }
    else
    {
        std::vector<DataType> args = {dp.getType()};
        return this->value_object->getVariable("`+").getLambda(args).evaluate({dp});
    }

    return res;
}
DataPoint DataPoint::sub(const DataPoint &dp) const
{
    if (dpn == VAR_REF)
        return value_ref->sub(dp);

    DataPoint res;
    res.dt = this->dt;

    if (dt.equals("int"))
        res.value_int = this->getInt() - dp.getInt();
    else if (dt.equals("long"))
        res.value_long = this->getLong() - dp.getLong();
    else if (dt.equals("short"))
        res.value_short = this->getShort() - dp.getShort();
    else if (dt.equals("char"))
        res.value_char = this->getChar() - dp.getChar();
    else if (dt.equals("bool"))
        res.value_boolean = this->getBool() - dp.getBool();
    else if (dt.equals("double"))
        res.value_double = this->getDouble() - dp.getDouble();
    else if (dt.equals("float"))
        res.value_float = this->getFloat() - dp.getFloat();
    else
    {
        std::vector<DataType> args = {dp.getType()};
        return this->value_object->getVariable("`-").getLambda(args).evaluate({dp});
    }

    return res;
}
DataPoint DataPoint::mul(const DataPoint &dp) const
{
    if (dpn == VAR_REF)
        return value_ref->mul(dp);

    DataPoint res;
    res.dt = this->dt;

    if (dt.equals("int"))
        res.value_int = this->getInt() * dp.getInt();
    else if (dt.equals("long"))
        res.value_long = this->getLong() * dp.getLong();
    else if (dt.equals("short"))
        res.value_short = this->getShort() * dp.getShort();
    else if (dt.equals("char"))
        res.value_char = this->getChar() * dp.getChar();
    else if (dt.equals("bool"))
        res.value_boolean = this->getBool() * dp.getBool();
    else if (dt.equals("double"))
        res.value_double = this->getDouble() * dp.getDouble();
    else if (dt.equals("float"))
        res.value_float = this->getFloat() * dp.getFloat();
    else
    {
        std::vector<DataType> args = {dp.getType()};
        return this->value_object->getVariable("`*").getLambda(args).evaluate({dp});
    }

    return res;
}
DataPoint DataPoint::div(const DataPoint &dp) const
{
    if (dpn == VAR_REF)
        return value_ref->div(dp);

    DataPoint res;
    res.dt = this->dt;

    if (dt.equals("int"))
        res.value_int = this->getInt() / dp.getInt();
    else if (dt.equals("long"))
        res.value_long = this->getLong() / dp.getLong();
    else if (dt.equals("short"))
        res.value_short = this->getShort() / dp.getShort();
    else if (dt.equals("char"))
        res.value_char = this->getChar() / dp.getChar();
    else if (dt.equals("bool"))
        res.value_boolean = this->getBool() / dp.getBool();
    else if (dt.equals("double"))
        res.value_double = this->getDouble() / dp.getDouble();
    else if (dt.equals("float"))
        res.value_float = this->getFloat() / dp.getFloat();
    else
    {
        std::vector<DataType> args = {dp.getType()};
        return this->value_object->getVariable("`/").getLambda(args).evaluate({dp});
    }

    return res;
}
DataPoint DataPoint::mod(const DataPoint &dp) const
{
    if (dpn == VAR_REF)
        return value_ref->div(dp);

    DataPoint res;
    res.dt = this->dt;

    if (dt.equals("int"))
        res.value_int = this->getInt() % dp.getInt();
    else if (dt.equals("long"))
        res.value_long = this->getLong() % dp.getLong();
    else if (dt.equals("short"))
        res.value_short = this->getShort() % dp.getShort();
    else if (dt.equals("char"))
        res.value_char = this->getChar() % dp.getChar();
    else if (dt.equals("bool"))
        res.value_boolean = this->getBool() % dp.getBool();
    else if (dt.equals("double"))
        res.value_double = (int)this->getDouble() % (int)dp.getDouble();
    else if (dt.equals("float"))
        res.value_float = (int)this->getFloat() % (int)dp.getFloat();
    else
    {
        std::vector<DataType> args = {dp.getType()};
        return this->value_object->getVariable("`%").getLambda(args).evaluate({dp});
    }

    return res;
}
DataPoint DataPoint::pow(const DataPoint &dp) const
{
    if (dpn == VAR_REF)
        return value_ref->pow(dp);

    DataPoint res;
    res.dt = this->dt;

    if (dt.equals("int"))
        res.value_int = std::pow(this->getInt(), dp.getInt());
    else if (dt.equals("long"))
        res.value_long = std::pow(this->getLong(), dp.getLong());
    else if (dt.equals("short"))
        res.value_short = std::pow(this->getShort(), dp.getShort());
    else if (dt.equals("char"))
        res.value_char = std::pow(this->getChar(), dp.getChar());
    else if (dt.equals("bool"))
        res.value_boolean = std::pow(this->getBool(), dp.getBool());
    else if (dt.equals("double"))
        res.value_double = std::pow(this->getDouble(), dp.getDouble());
    else if (dt.equals("float"))
        res.value_float = std::pow(this->getFloat(), dp.getFloat());
    else
    {
        std::vector<DataType> args = {dp.getType()};
        return this->value_object->getVariable("`**").getLambda(args).evaluate({dp});
    }

    return res;
}
DataPoint DataPoint::less(const DataPoint &dp) const
{
    if (dpn == VAR_REF)
        return value_ref->less(dp);

    DataPoint res;
    res.dt = DataType("bool");

    if (dt.equals("int"))
        res.value_boolean = this->getInt() < dp.getInt();
    else if (dt.equals("long"))
        res.value_boolean = this->getLong() < dp.getLong();
    else if (dt.equals("short"))
        res.value_boolean = this->getShort() < dp.getShort();
    else if (dt.equals("char"))
        res.value_boolean = this->getChar() < dp.getChar();
    else if (dt.equals("bool"))
        res.value_boolean = this->getBool() < dp.getBool();
    else if (dt.equals("double"))
        res.value_boolean = this->getDouble() < dp.getDouble();
    else if (dt.equals("float"))
        res.value_boolean = this->getFloat() < dp.getFloat();
    else
    {
        std::vector<DataType> args = {dp.getType()};
        return this->value_object->getVariable("`<").getLambda(args).evaluate({dp});
    }

    return res;
}
DataPoint DataPoint::more(const DataPoint &dp) const
{
    if (dpn == VAR_REF)
        return value_ref->more(dp);

    DataPoint res;
    res.dt = DataType("bool");

    if (dt.equals("int"))
        res.value_boolean = this->getInt() > dp.getInt();
    else if (dt.equals("long"))
        res.value_boolean = this->getLong() > dp.getLong();
    else if (dt.equals("short"))
        res.value_boolean = this->getShort() > dp.getShort();
    else if (dt.equals("char"))
        res.value_boolean = this->getChar() > dp.getChar();
    else if (dt.equals("bool"))
        res.value_boolean = this->getBool() > dp.getBool();
    else if (dt.equals("double"))
        res.value_boolean = this->getDouble() > dp.getDouble();
    else if (dt.equals("float"))
        res.value_boolean = this->getFloat() > dp.getFloat();
    else
    {
        std::vector<DataType> args = {dp.getType()};
        return this->value_object->getVariable("`>").getLambda(args).evaluate({dp});
    }

    return res;
}
DataPoint DataPoint::eless(const DataPoint &dp) const
{
    if (dpn == VAR_REF)
        return value_ref->eless(dp);

    DataPoint res;
    res.dt = DataType("bool");

    if (dt.equals("int"))
        res.value_boolean = this->getInt() <= dp.getInt();
    else if (dt.equals("long"))
        res.value_boolean = this->getLong() <= dp.getLong();
    else if (dt.equals("short"))
        res.value_boolean = this->getShort() <= dp.getShort();
    else if (dt.equals("char"))
        res.value_boolean = this->getChar() <= dp.getChar();
    else if (dt.equals("bool"))
        res.value_boolean = this->getBool() <= dp.getBool();
    else if (dt.equals("double"))
        res.value_boolean = this->getDouble() <= dp.getDouble();
    else if (dt.equals("float"))
        res.value_boolean = this->getFloat() <= dp.getFloat();
    else
    {
        std::vector<DataType> args = {dp.getType()};
        return this->value_object->getVariable("`<=").getLambda(args).evaluate({dp});
    }

    return res;
}
DataPoint DataPoint::emore(const DataPoint &dp) const
{
    if (dpn == VAR_REF)
        return value_ref->emore(dp);

    DataPoint res;
    res.dt = DataType("bool");

    if (dt.equals("int"))
        res.value_boolean = this->getInt() >= dp.getInt();
    else if (dt.equals("long"))
        res.value_boolean = this->getLong() >= dp.getLong();
    else if (dt.equals("short"))
        res.value_boolean = this->getShort() >= dp.getShort();
    else if (dt.equals("char"))
        res.value_boolean = this->getChar() >= dp.getChar();
    else if (dt.equals("bool"))
        res.value_boolean = this->getBool() >= dp.getBool();
    else if (dt.equals("double"))
        res.value_boolean = this->getDouble() >= dp.getDouble();
    else if (dt.equals("float"))
        res.value_boolean = this->getFloat() >= dp.getFloat();
    else
    {
        std::vector<DataType> args = {dp.getType()};
        return this->value_object->getVariable("`>=").getLambda(args).evaluate({dp});
    }

    return res;
}
DataPoint DataPoint::equals(DataPoint dp) const
{
    if (dpn == VAR_REF)
        return value_ref->equals(dp);

    DataPoint res;
    res.dt = DataType("bool");
    res.value_boolean = false;

    if (dt.equals("int"))
        res.value_boolean = this->getInt() == dp.getInt();
    else if (dt.equals("long"))
        res.value_boolean = this->getLong() == dp.getLong();
    else if (dt.equals("short"))
        res.value_boolean = this->getShort() == dp.getShort();
    else if (dt.equals("char"))
        res.value_boolean = this->getChar() == dp.getChar();
    else if (dt.equals("bool"))
        res.value_boolean = this->getBool() == dp.getBool();
    else if (dt.equals("double"))
        res.value_boolean = this->getDouble() == dp.getDouble();
    else if (dt.equals("float"))
        res.value_boolean = this->getFloat() == dp.getFloat();
    else if (dt.is_array && dp.getType().is_array)
    {
        if (this->value_vector.size() != dp.getVector().size())
            res.value_boolean = false;
        else
        {
            res.value_boolean = true;
            for (int i = 0; i < this->value_vector.size(); i++)
            {
                if (!this->value_vector[i].equals(dp.getVector()[i]).getBool())
                {
                    res.value_boolean = false;
                    break;
                }
            }
        }
        return res;
    }
    else
    {
        std::vector<DataType> args = {dp.getType()};
        return this->value_object->getVariable("`==").getLambda(args).evaluate({dp});
    }

    return res;
}
DataPoint DataPoint::nequals(DataPoint dp) const
{
    if (dpn == VAR_REF)
        return value_ref->nequals(dp);

    DataPoint res;
    res.dt = DataType("bool");
    res.value_boolean = true;

    if (dt.equals("int"))
        res.value_boolean = this->getInt() != dp.getInt();
    else if (dt.equals("long"))
        res.value_boolean = this->getLong() != dp.getLong();
    else if (dt.equals("short"))
        res.value_boolean = this->getShort() != dp.getShort();
    else if (dt.equals("char"))
        res.value_boolean = this->getChar() != dp.getChar();
    else if (dt.equals("bool"))
        res.value_boolean = this->getBool() != dp.getBool();
    else if (dt.equals("double"))
        res.value_boolean = this->getDouble() != dp.getDouble();
    else if (dt.equals("float"))
        res.value_boolean = this->getFloat() != dp.getFloat();
    else if (dt.is_array && dp.getType().is_array)
    {
        if (this->value_vector.size() != dp.getVector().size())
            res.value_boolean = true;
        else
        {
            res.value_boolean = false;
            for (int i = 0; i < this->value_vector.size(); i++)
            {
                if (!this->value_vector[i].equals(dp.getVector()[i]).getBool())
                {
                    res.value_boolean = true;
                    break;
                }
            }
        }
        return res;
    }
    else
    {
        std::vector<DataType> args = {dp.getType()};
        return this->value_object->getVariable("`!=").getLambda(args).evaluate({dp});
    }

    return res;
}

const int DataPoint::getInt() const
{
    if (dpn == VAR_REF)
        return value_ref->getInt();

    if (dt.equals("int"))
        return value_int;
    else if (dt.equals("long"))
        return value_long;
    else if (dt.equals("short"))
        return value_short;
    else if (dt.equals("char"))
        return value_char;
    else if (dt.equals("bool"))
        return value_boolean;
    else if (dt.equals("double"))
        return value_double;
    else if (dt.equals("float"))
        return value_float;

    throw std::runtime_error("Error: invalid int type: " + getDebug());
}
const long DataPoint::getLong() const
{
    if (dpn == VAR_REF)
        return value_ref->getLong();

    if (dt.equals("int"))
        return value_int;
    else if (dt.equals("long"))
        return value_long;
    else if (dt.equals("short"))
        return value_short;
    else if (dt.equals("char"))
        return value_char;
    else if (dt.equals("bool"))
        return value_boolean;
    else if (dt.equals("double"))
        return value_double;
    else if (dt.equals("float"))
        return value_float;

    throw std::runtime_error("Error: invalid long type.");
}
const double DataPoint::getDouble() const
{
    if (dpn == VAR_REF)
        return value_ref->getDouble();

    if (dt.equals("int"))
        return value_int;
    else if (dt.equals("long"))
        return value_long;
    else if (dt.equals("short"))
        return value_short;
    else if (dt.equals("char"))
        return value_char;
    else if (dt.equals("bool"))
        return value_boolean;
    else if (dt.equals("double"))
        return value_double;
    else if (dt.equals("float"))
        return value_float;

    throw std::runtime_error("Error: invalid double type.");
}
const float DataPoint::getFloat() const
{
    if (dpn == VAR_REF)
        return value_ref->getFloat();

    if (dt.equals("int"))
        return value_int;
    else if (dt.equals("long"))
        return value_long;
    else if (dt.equals("short"))
        return value_short;
    else if (dt.equals("char"))
        return value_char;
    else if (dt.equals("bool"))
        return value_boolean;
    else if (dt.equals("double"))
        return value_double;
    else if (dt.equals("float"))
        return value_float;

    throw std::runtime_error("Error: invalid float type.");
}
const bool DataPoint::getBool() const
{
    if (dpn == VAR_REF)
        return value_ref->getBool();

    if (dt.equals("int"))
        return value_int;
    else if (dt.equals("long"))
        return value_long;
    else if (dt.equals("short"))
        return value_short;
    else if (dt.equals("char"))
        return value_char;
    else if (dt.equals("bool"))
        return value_boolean;
    else if (dt.equals("double"))
        return value_double;
    else if (dt.equals("float"))
        return value_float;

    throw std::runtime_error("Error: invalid bool type.");
}
const short DataPoint::getShort() const
{
    if (dpn == VAR_REF)
        return value_ref->getShort();

    if (dt.equals("int"))
        return value_int;
    else if (dt.equals("long"))
        return value_long;
    else if (dt.equals("short"))
        return value_short;
    else if (dt.equals("char"))
        return value_char;
    else if (dt.equals("bool"))
        return value_boolean;
    else if (dt.equals("double"))
        return value_double;
    else if (dt.equals("float"))
        return value_float;

    throw std::runtime_error("Error: invalid short type.");
}
const char DataPoint::getChar() const
{
    if (dpn == VAR_REF)
        return value_ref->getChar();

    if (dt.equals("int"))
        return value_int;
    else if (dt.equals("long"))
        return value_long;
    else if (dt.equals("short"))
        return value_short;
    else if (dt.equals("char"))
        return value_char;
    else if (dt.equals("bool"))
        return value_boolean;
    else if (dt.equals("double"))
        return value_double;
    else if (dt.equals("float"))
        return value_float;

    throw std::runtime_error("Error: invalid char type.");
}

const std::string DataPoint::getDebug() const
{
    std::string s;
    switch (dpn)
    {
    case END_NODE:
        s = dt.getString() + "::";
        if (dt.equals("int"))
            s += std::to_string(value_int);
        else if (dt.equals("long"))
            s += std::to_string(value_long);
        else if (dt.equals("short"))
            s += std::to_string(value_short);
        else if (dt.equals("char"))
            s += std::to_string(value_char);
        else if (dt.equals("bool"))
            s += std::to_string(value_boolean);
        else if (dt.equals("double"))
            s += std::to_string(value_double);
        else if (dt.equals("float"))
            s += std::to_string(value_float);
        else if (dt.equals("void"))
            s += (POINTER == NULL ? "NULL" : "SET");
        else if (dt.is_array)
        {
            s += "[ ";
            for (auto &d : value_vector)
                s += d.getDebug() + " ";
            s += "]";
        }
        else if (dt.is_lambda && !poly_lambda.empty())
        {
            s += "LAM";
        }
        else
        {
            s += (value_object == NULL ? "PROTO" : "OBJ");
        }
        break;
    case VAR_REF:
        s = "REF::" + value_ref->getDebug();
        break;
    }
    return s;
}

void DataPoint::set(DataPoint dp)
{
    //std::cout << "SET\t" << getDebug() << "\t" << dp.getDebug() << "\n";
    if (dp.dpn == VAR_REF)
    {
        return this->set(*dp.value_ref);
    }
    else if (dpn == VAR_REF)
    {
        return this->value_ref->set(dp);
    }

    if (dp.getObject() != NULL && dp.getObject()->variables.find("@") != dp.getObject()->variables.end())
    {
        auto lambda = dp.getObject()->variables["@"].poly_lambda;
        Lambda candidate(NULL, Node(), {}, {}, DataType());
        for (auto &l : lambda)
        {
            if (l.return_type.equals(dt))
            {
                candidate = l;
                break;
            }
        }
        if (candidate.parent != NULL)
        {
            dp = candidate.evaluate({});
        }
    }

    if (dt.equals("int"))
        this->value_int = dp.getInt();
    else if (dt.equals("long"))
        this->value_long = dp.getLong();
    else if (dt.equals("short"))
        this->value_short = dp.getShort();
    else if (dt.equals("char"))
        this->value_char = dp.getChar();
    else if (dt.equals("bool"))
        this->value_boolean = dp.getBool();
    else if (dt.equals("double"))
        this->value_double = dp.getDouble();
    else if (dt.equals("float"))
        this->value_float = dp.getFloat();
    else if (dt.equals("void"))
        this->POINTER = dp.getPointer();
    else if (dt.is_array && dp.getType().is_array)
    {
        if (value_vector.size() != dp.value_vector.size() && (value_vector.empty()) || (!dt.arr_dimensions.empty() && dt.arr_dimensions[0] == 0))
        {
            value_vector.clear();
            //dt.arr_dimensions = dp.dt.arr_dimensions;
            for (int i = 0; i < dp.value_vector.size(); i++)
            {
                value_vector.push_back(DataPoint(dt.getSubType()));
                value_vector.back().set(DataPoint(dp.value_vector[i]));
            }
        }
        else
        {
            for (int i = 0; i < value_vector.size(); i++)
            {
                value_vector[i].set(dp.value_vector[i]);
            }
        }
    }
    else if (dt.is_lambda && dp.getType().is_lambda)
    {
        if (this->dt.return_type == dp.dt.return_type)
        {
            this->poly_lambda = dp.poly_lambda;
        }
    }
    else
    {
        if (this->value_object != NULL)
        {
            if (this->value_object->variables.find("`=") != this->value_object->variables.end())
            {
                std::vector<DataType> args = {dp.getType()};
                this->value_object->getVariable("`=").getLambda(args).evaluate({dp});
            }
            else
                setObject(dp.value_object);
        }
        else if (this->value_object == NULL && !dp.getType().equals(dt))
        {
            if (dt.proto_object->variables.find("init") != dt.proto_object->variables.end())
            {
                std::vector<DataType> params = {dp.getType()};
                if (dt.proto_object->getVariable("init").getLambda(params).parent != NULL)
                {
                    Scope * object_scope = new Scope(getType().proto_object->parent);
                    getType().prototype.back().evaluate(object_scope);
                    setObject(object_scope);
                    getObject()->declareVariable("self", getType());
                    getObject()->variables["self"].set(this);
                    auto args = {dp};
                    getObject()->getVariable("init").getLambda(params).evaluate(args);
                }
            }
        }
        else if (dp.getType().equals(dt))
        {
            setObject(dp.value_object);
        }
        else
        {
            throw std::runtime_error(("Error: Incompatible Objects " + getDebug() + ", " + dp.getDebug() + "\n").c_str());
        }
    }
}

Lambda DataPoint::getLambda(std::vector<DataType> args)
{
    if (dpn == VAR_REF)
        return value_ref->getLambda(args);

    std::vector<Lambda> possible;
    for (auto &lambda : poly_lambda)
    {
        bool absolute = true;
        bool flag = true;
        if (args.size() != lambda.params.size())
            continue;
        for (int i = 0; i < args.size(); i++)
        {
            DataType current_l = args[i];
            DataType current_r = lambda.params[i];
            if (current_l.equals(current_r))
                continue;
            else
                absolute = false;
            while (flag)
            {
                if (current_l.isBaseNumber() && current_r.is_array)
                {
                    flag = false;
                    continue;
                }
                else if (current_r.isBaseNumber() && current_l.is_array)
                {
                    flag = false;
                    continue;
                }
                else if (current_l.is_lambda != current_r.is_lambda)
                {
                    flag = false;
                    continue;
                }
                else if (!current_l.prototype.empty() && current_l.proto_object->variables.find("@") != current_l.proto_object->variables.end())
                {
                    auto lambda = current_l.proto_object->variables["@"].poly_lambda;
                    Lambda candidate(NULL, Node(), {}, {}, DataType());
                    for (auto &l : lambda)
                    {
                        if (l.return_type.equals(current_r))
                        {
                            candidate = l;
                            break;
                        }
                    }
                    if (candidate.parent == NULL)
                    {
                        if (!current_r.isBaseNumber() && !current_r.is_array)
                        {
                            if (!current_r.prototype.empty())
                            {
                                if (current_r.proto_object->variables.find("init") != current_r.proto_object->variables.end())
                                {
                                    DataPoint init = current_r.proto_object->variables["init"];
                                    if (init.getLambda({current_l}).parent == NULL)
                                    {
                                        flag = false;
                                    }
                                }
                                else
                                {
                                    flag = false;
                                }
                            } else {
                                flag = false;
                            }
                        } else {
                            flag = false;
                        }
                    }
                }

                if (current_r.getSubType().equals("UNDEF") || current_l.getSubType().equals("UNDEF"))
                {
                    break;
                }
                current_l = current_l.getSubType();
                current_r = current_r.getSubType();
            }
            if (!flag)
                break;
        }
        if (absolute)
            return lambda;
        if (flag)
            possible.push_back(lambda);
    }
    if (possible.empty())
    {
        return Lambda(NULL, Node(), {}, {}, DataType());
        //throw std::runtime_error("Invalid lambda arguments, no overloaded function found.");
    }

    return possible[0];
}

std::vector<DataPoint> DataPoint::getVector() const
{
    if (dpn == VAR_REF)
        return value_ref->getVector();
    return value_vector;
}

const DataType DataPoint::getType() const
{
    if (dpn == VAR_REF)
        return value_ref->getType();
    return dt;
}

void DataPoint::setObject(Scope *value_object)
{
    if (dpn == VAR_REF)
        return value_ref->setObject(value_object);
    if (this->value_object != NULL)
        this->value_object->references--;
    this->value_object = value_object;
    this->value_object->references++;
}

Scope *DataPoint::getObject()
{
    if (dpn == VAR_REF)
        return value_ref->getObject();
    if (this->value_object == NULL)
        return this->dt.proto_object;
    return this->value_object;
}

void DataPoint::addLambda(Lambda lambda)
{
    if (dpn == VAR_REF)
        return value_ref->addLambda(lambda);
    poly_lambda.push_back(lambda);
}

DataPoint DataPoint::clone(Scope *new_parent) const
{
    DataPoint dp(*this);
    if (this->dt.is_lambda)
    {
        dp.poly_lambda.clear();
        for (auto &l : poly_lambda)
        {
            dp.poly_lambda.push_back(l.clone(new_parent));
        }
    }
    return dp;
}

DataPoint DataPoint::index(std::vector<DataPoint> args)
{
    if (dpn == VAR_REF)
        return value_ref->index(args);
    if (dt.is_array)
        return DataPoint(&value_vector[args.back().getLong()]);
    else
    {
        std::vector<DataType> params;
        for (auto &p : args)
            params.push_back(p.getType());
        return this->value_object->getVariable("`[]").getLambda(params).evaluate(args);
    }
}

void *DataPoint::getPointer() const
{
    if (dpn == VAR_REF)
        return value_ref->getPointer();
    return this->POINTER;
}

DataPoint::~DataPoint()
{
    if (this->value_object != NULL)
    {
        this->value_object->references--;
    }
}