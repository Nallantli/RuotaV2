#ifndef RUOTA_H
#define RUOTA_H

#define RUOTA_VERSION	"1.0.2 Alpha"

#include <vector>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <fstream>
#include "Tokenizer.h"
#include <boost/filesystem.hpp>
#include <boost/asio.hpp>
#include <boost/array.hpp>

struct DataType;
struct DataPoint;
struct Node;
struct Scope;
struct Lambda;
struct RuotaWrapper;

enum DataPointNode
{
	VAR_REF,
	END_NODE
};

enum NodeType
{
	NULL_NODE,

	VALUE,
	SEQUENCE,
	VARIABLE,
	SCOPE,
	SEQUENCE_LAX,
	LAM_DEC,
	RETURN,
	U_NEW,

	B_REG,
	B_GENERIC,
	B_TEMP,

	B_EXEC,
	B_ADD,
	B_SUB,
	B_MUL,
	B_DIV,
	B_DEC,
	B_MOD,
	B_POW,
	B_SET,
	B_INDEX,
	B_OBJ,
	B_STATIC,
	B_OBJ_INDEX,
	B_ITER,
	B_DO_LOOP,
	B_RANGE,
	B_RANGE_INDIC,
	B_IF_ELSE,

	B_LESS,
	B_ELESS,
	B_MORE,
	B_EMORE,
	B_EQUALS,
	B_NEQUALS,

	B_OR,
	B_AND,

	FLAG_POINTER,
	U_LEN
};

struct DataType
{
	std::string data_type;
	std::vector<Node> prototype;
	Scope * proto_object = NULL;
	std::vector<DataType> qualifiers;
	bool is_array = false;
	std::vector<unsigned long long> arr_dimensions;

	std::vector<DataType> return_type;
	bool is_lambda = false;
	std::vector<DataType> params;

	DataType(const DataType &dt);

	// NON-LAMBDA
	DataType(
		std::string data_type = "null",
		std::vector<DataType> qualifiers = {},
		bool is_array = false,
		std::vector<unsigned long long> arr_dimensions = {});

	DataType(
		std::vector<Node> prototype,
		Scope * proto_object,
		std::string data_type,
		std::vector<DataType> qualifiers = {},
		bool is_array = false,
		std::vector<unsigned long long> arr_dimensions = {});
	// LAMBDA
	DataType(
		std::vector<DataType> return_type,
		std::vector<DataType> params);
	const std::string getString() const;
	const DataType getSubType() const;
	const bool equals(const DataType &dt) const;
	const bool equals(const std::string &s) const;
	const bool operator==(const DataType &dt) const;
	const unsigned long long getFullDim() const;
	const bool isBaseNumber() const;
	Scope * getProtoObject();
	~DataType();
};

struct DataPoint
{
	union {
		int value_int;
		long value_long;
		double value_double;
		float value_float;
		bool value_boolean;
		short value_short;
		char value_char;
		//Lambda * value_lambda;
	};
	void *POINTER = NULL;
	Scope *value_object = NULL;
	DataPoint *value_ref = NULL;
	bool flag_return = false;
	bool flag_pointer = false;
	std::vector<DataPoint> value_vector;
	std::vector<Lambda> poly_lambda;
	DataPointNode dpn;
	DataType dt;
	DataPoint();
	DataPoint(const DataPoint &dp);
	DataPoint(DataPoint *dp);
	DataPoint(void *);
	DataPoint(std::vector<DataPoint>);
	/*DataPoint(int value_int);
    DataPoint(long value_long);
    DataPoint(double value_double);
    DataPoint(float value_float);
    DataPoint(short value_short);
    DataPoint(char value_char);*/
	DataPoint(DataType);
	DataPoint(int);
	DataPoint(long);
	DataPoint(short);
	DataPoint(char);
	DataPoint(float);
	DataPoint(double);
	DataPoint(bool);
	DataPoint add(const DataPoint &dp) const;
	DataPoint sub(const DataPoint &dp) const;
	DataPoint mul(const DataPoint &dp) const;
	DataPoint div(const DataPoint &dp) const;
	DataPoint mod(const DataPoint &dp) const;
	DataPoint pow(const DataPoint &dp) const;
	DataPoint less(const DataPoint &dp) const;
	DataPoint more(const DataPoint &dp) const;
	DataPoint eless(const DataPoint &dp) const;
	DataPoint emore(const DataPoint &dp) const;
	DataPoint equals(DataPoint dp) const;
	DataPoint nequals(DataPoint dp) const;
	Lambda getLambda(std::vector<DataType>);
	std::vector<DataPoint> getVector() const;
	const int getInt() const;
	const long getLong() const;
	const double getDouble() const;
	const float getFloat() const;
	const bool getBool() const;
	const short getShort() const;
	const char getChar() const;
	void *getPointer() const;

	void set(DataPoint dp);
	void addLambda(Lambda lambda);
	void setObject(Scope *value_object);
	Scope *getObject();

	const std::string getDebug() const;
	DataType getType() const;
	DataPoint clone(Scope *) const;
	DataPoint index(std::vector<DataPoint>);
	~DataPoint();
};

struct Node
{
	NodeType nt;
	std::vector<Node> params;
	std::vector<std::string> values;
	DataPoint dp;
	Node();
	Node(const Node &);
	Node(DataPoint);
	Node(NodeType, std::vector<std::string> values);
	Node(NodeType, std::vector<Node>);
	DataPoint evaluate(Scope *) const;
};

struct Scope
{
	bool stat = false;
	unsigned long long references = 0;
	std::unordered_map<std::string, DataPoint> variables;
	std::vector<std::pair<std::string, DataType>> generics;
	Scope *parent;
	Scope(Scope *);
	DataPoint getVariable(const std::string &);
	DataPoint declareVariable(const std::string &, DataType);
	const std::string getDebug() const;
	Scope *clone() const;
};

struct Lambda
{
	Node main;
	Scope *parent;
	std::vector<DataType> params;
	std::vector<std::string> keys;
	DataType return_type;
	Lambda(Scope *, Node, std::vector<DataType>, std::vector<std::string>, DataType);
	DataPoint evaluate(std::vector<DataPoint>) const;
	Lambda clone(Scope *) const;
};

struct RuotaWrapper
{
	static std::string path;
	static std::string curr_file;
	static std::string current_dir;
	Tokenizer *T;
	static std::unordered_map<std::string, int> operators;
	Scope *MAIN;
	std::vector<std::string> LOADED;
	RuotaWrapper(std::string);
	static std::unordered_map<std::string, std::vector<DataPoint> (*)(std::vector<DataPoint>)> registers;
	static void register_command(std::string key, std::vector<DataPoint> (*value)(std::vector<DataPoint>));
	const DataPoint evaluate(const std::string &line);
	Node parseLine(const std::string &line, const std::string &local_file);
	
	~RuotaWrapper();
};

static const std::string data_to_string(const DataPoint &dp)
{
	std::string d = "";
	for (auto &e : dp.getVector())
	{
		d += std::string(1, e.getChar());
	}
	return d;
}

static DataPoint string_to_data(const std::string &s)
{
	std::vector<DataPoint> dp;
	for (auto &c : s)
		dp.push_back(DataPoint(c));
	DataPoint r(dp);
	r.dt.is_array = true;
	return r;
}

#endif