#pragma once
#include "Common.h"



enum eField
{
	NAME, ADDRESS, PHONE_NUMBER, FIELD_COUNT
};

enum eLogicOperator
{
	AND, OR, LOGIC_OPERATOR_COUNT
};


const std::unordered_map<eField, std::string> FIELD_TO_STRING_TABLE = {
	{NAME, "Name"},
	{ADDRESS, "Address"},
	{PHONE_NUMBER, "Phone Number"}
};
const std::unordered_map<eLogicOperator, std::string> LOGIC_OP_TO_STRING_TABLE = {
	{AND, "and"},
	{OR, "or"}
};


class Condition
{
public:
	Condition(const eField field, const std::string& value) : mField(field), mValue(value) {}
	const eField GetKeyType() const;
	const std::string& GetValue() const;
	void Print() const;

private:
	eField mField;
	std::string mValue;
};

