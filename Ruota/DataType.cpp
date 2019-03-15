#include "Ruota.h"

DataType::DataType(const DataType &dt)
{
	this->prototype = dt.prototype;
	this->proto_object = dt.proto_object;
	this->data_type = dt.data_type;
	this->qualifiers = dt.qualifiers;
	this->is_array = dt.is_array;
	this->arr_dimensions = dt.arr_dimensions;
	this->is_lambda = dt.is_lambda;
	this->return_type = dt.return_type;
	this->params = dt.params;
}

// NON-LAMBDA
DataType::DataType(
	std::string data_type,
	std::vector<DataType> qualifiers,
	bool is_array,
	std::vector<unsigned long long> arr_dimensions)
{
	this->data_type = data_type;
	this->qualifiers = qualifiers;
	this->is_array = is_array;
	this->arr_dimensions = arr_dimensions;
	this->is_lambda = false;
}

DataType::DataType(
	std::vector<Node> prototype,
	Scope *proto_object,
	std::string data_type,
	std::vector<DataType> qualifiers,
	bool is_array,
	std::vector<unsigned long long> arr_dimensions)
{
	this->prototype = prototype;
	this->proto_object = proto_object;
	if (this->proto_object != NULL)
		this->proto_object->references++;
	this->data_type = data_type;
	this->qualifiers = qualifiers;
	this->is_array = is_array;
	this->arr_dimensions = arr_dimensions;
	this->is_lambda = false;
}
// LAMBDA
DataType::DataType(
	std::vector<DataType> return_type,
	std::vector<DataType> params)
{
	this->is_lambda = true;
	this->return_type = return_type;
	this->params = params;
}

const std::string DataType::getString() const
{
	std::string s = "";
	if (!is_lambda)
	{
		s += data_type;
		if (!qualifiers.empty())
		{
			s += "<" + qualifiers[0].getString();
			for (int i = 1; i < qualifiers.size(); i++)
				s += "," + qualifiers[i].getString();
			s += ">";
		}
		if (is_array)
		{
			if (arr_dimensions.empty())
				s += "[]";
			else
			{
				s += "[" + std::to_string(arr_dimensions[0]);
				for (int i = 1; i < arr_dimensions.size(); i++)
					s += "," + std::to_string(arr_dimensions[i]);
				s += "]";
			}
		}
	}
	else
	{
		s = "(" + return_type[0].getString() + " <- ";
		if (!params.empty())
		{
			s += "[" + params[0].getString();
			for (int i = 1; i < params.size(); i++)
				s += "," + params[i].getString();
			s += "]";
		}
		else
			s += "[]";
		s += ")";
	}
	return s;
}

const DataType DataType::getSubType() const
{
	if (arr_dimensions.empty())
		return DataType("UNDEF");
	std::vector<unsigned long long> new_dims;
	for (int i = 1; i < arr_dimensions.size(); i++)
		new_dims.push_back(arr_dimensions[i]);
	if (new_dims.empty())
		return DataType(prototype, proto_object, data_type, qualifiers);
	else
		return DataType(prototype, proto_object, data_type, qualifiers, true, new_dims);
}

const bool DataType::equals(const DataType &dt) const
{
	return equals(dt.getString());
}

const bool DataType::equals(const std::string &s) const
{
	return getString() == s;
}

const bool DataType::operator==(const DataType &dt) const
{
	return this->equals(dt);
}

const unsigned long long DataType::getFullDim() const
{
	if (arr_dimensions.empty())
		return 0;
	unsigned long long mul = 1;
	for (auto &d : arr_dimensions)
		mul *= d;
	return mul;
}

const bool DataType::isBaseNumber() const
{
	if (is_array || is_lambda)
		return false;
	switch (str2int(data_type.c_str()))
	{
	case str2int("int"):
	case str2int("long"):
	case str2int("short"):
	case str2int("char"):
	case str2int("bool"):
	case str2int("double"):
	case str2int("float"):
	case str2int("void"):
		return true;
	default:
		return false;
	}
}

Scope *DataType::getProtoObject()
{
	return proto_object;
}

DataType::~DataType()
{
	if (proto_object != NULL)
	{
		proto_object->references--;
		//std::cout << "REF\t" << proto_object->references << "\t" << getString() << std::endl;
	}
}