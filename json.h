#pragma once

#include <iostream>
#include <map>
#include <string>
#include <variant>
#include <vector>

namespace json
{

class Node;

using std::string;
using std::string;
using std::map;
using std::vector;
using std::runtime_error;
using std::logic_error;
using std::variant;
using std::istream;
using std::holds_alternative;
using std::get;
using namespace std::string_literals;
using namespace std::literals;

using Dict = map<string, Node>;
using Array = vector<Node>;

class ParsingError: public runtime_error
{
public:
	using runtime_error::runtime_error;
};

class Node final:
		private variant<nullptr_t, Array, Dict, bool, int, double, string>
{
public:
	using variant::variant;
	using Value = variant;

	bool IsInt() const
	{
		return holds_alternative<int>(*this);
	}

	int AsInt() const
	{
		if (!IsInt()) {
			throw logic_error("Not an int"s);
		}
		return get<int>(*this);
	}

	bool IsPureDouble() const
	{
		return holds_alternative<double>(*this);
	}

	bool IsDouble() const
	{
		return IsInt() || IsPureDouble();
	}

	double AsDouble() const
	{
		if (!IsDouble()) {
			throw logic_error("Not a double"s);
		}
		return IsPureDouble() ? get<double>(*this) : AsInt();
	}

	bool IsBool() const
	{
		return holds_alternative<bool>(*this);
	}

	bool AsBool() const
	{
		if (!IsBool()) {
			throw logic_error("Not a bool"s);
		}
		return get<bool>(*this);
	}

	bool IsNull() const
	{
		return holds_alternative<nullptr_t>(*this);
	}

	bool IsArray() const
	{
		return holds_alternative<Array>(*this);
	}

	const Array &AsArray() const
	{
		if (!IsArray()) {
			throw logic_error("Not an array"s);
		}
		return get<Array>(*this);
	}

	bool IsString() const
	{
		return holds_alternative<string>(*this);
	}

	const string &AsString() const
	{
		if (!IsString()) {
			throw logic_error("Not a string"s);
		}
		return get<string>(*this);
	}

	bool IsDict() const
	{
		return holds_alternative<Dict>(*this);
	}

	const Dict &AsDict() const
	{
		if (!IsDict()) {
			throw logic_error("Not a dict"s);
		}
		return get<Dict>(*this);
	}

	bool operator==(const Node &rhs) const
	{
		return GetValue() == rhs.GetValue();
	}

	const Value &GetValue() const
	{
		return *this;
	}
};

inline bool operator!=(const Node &lhs, const Node &rhs)
{
	return !(lhs == rhs);
}

class Document
{
public:
	explicit Document(Node root): root_(move(root)) {}

	const Node &GetRoot() const
	{
		return root_;
	}

private:
	Node root_;
};

inline bool operator==(const Document &lhs, const Document &rhs)
{
	return lhs.GetRoot() == rhs.GetRoot();
}

inline bool operator!=(const Document &lhs, const Document &rhs)
{
	return !(lhs == rhs);
}

Document Load(istream &input);

} // namespace json