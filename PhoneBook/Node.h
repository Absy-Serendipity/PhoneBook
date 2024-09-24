#pragma once
#include "Query.h"


enum eSymbolName {
	LOGIC_EXPR, EXPR, S, MORE_LOGIC_EXPR,
	VALUE, LOGIC_OP, EQUAL, FIELD, END_MARK
};
const std::unordered_map<std::string, eSymbolName> SYMBOL_NAME_TABLE = {
	{"or", LOGIC_OP},
	{"and", LOGIC_OP},
	{"name", FIELD},
	{"address", FIELD},
	{"phonenumber", FIELD},
};
const std::unordered_map<eSymbolName, std::string> SYMBOL_NAME_TO_STRING_TABLE = {
	{ LOGIC_EXPR, "LOGIC_EXPR" },
	{ EXPR, "EXPR" },
	{ S, "S" },
	{ MORE_LOGIC_EXPR, "MORE_LOGIC_EXPR" },
	{ VALUE, "VALUE" },
	{ LOGIC_OP, "LOGIC_OP" },
	{ EQUAL, "ASSIGN" },
	{ FIELD, "KEY" },
	{ END_MARK, "END_MARK" }
};

const std::unordered_map<std::string, eLogicOperator> LOGIC_OPERATOR_TABLE = {
	{"and", AND},
	{"or", OR}
};

const std::unordered_map<std::string, eField> FIELD_TABLE = {
	{"name", NAME},
	{"address", ADDRESS},
	{"phonenumber", PHONE_NUMBER}
};



class Node
{
public:
	Node(const Node& other) : mName(other.GetName()), mValue(other.GetValue()) {}
	Node(eSymbolName name, const std::string& value) : mName(name), mValue(value) {}
	Node(eSymbolName name) : mName(name) {}
	
	eSymbolName GetName() const;
	const std::string GetValue() const;

	virtual bool CreateQuery(Query& outQuery) const = 0;

private:
	eSymbolName mName;
	std::string mValue;
};

// name = 'null null' and address = "Seoul"
// Example of tokens for query above
// <FIELD, name>, <VALUE, null null>, <LOGIC_OP, and>
// <FIELD, address>, <VALUE, Seoul>
class Token : public Node
{
public:
	Token(const Token& other) : Node(other) {}
	Token(eSymbolName name) : Node(name) {}
	Token(eSymbolName name, const std::string& value) : Node(name, value) {}

	bool CreateQuery(Query&) const override;
};

// name = 'null null' and address = "Seoul"
// mExpression -> name = 'null null'
// mMoreLogicExpression -> and address = "Seoul"
class LogicExpression : public Node
{
public:
	LogicExpression(std::unique_ptr<Node> logicOperator, std::unique_ptr<Node> moreLogicExpression) : 
		Node(LOGIC_EXPR) , mExpression(std::move(logicOperator)), mMoreLogicExpression(std::move(moreLogicExpression)) {}

	bool CreateQuery(Query&) const override;

private:
	std::unique_ptr<Node> mExpression;
	std::unique_ptr<Node> mMoreLogicExpression;
};

// name = 'null null' and address = "Seoul"
// mLogicOp -> and
// mLogicExpression -> address = "Seoul"
class MoreLogicExpression : public Node
{
public: 
	MoreLogicExpression(std::unique_ptr<Node> logicOp, std::unique_ptr<Node> logicExpression) : 
		Node(MORE_LOGIC_EXPR), mLogicOp(std::move(logicOp)), mLogicExpression(std::move(logicExpression)) {}

	bool CreateQuery(Query&) const override;
private:
	std::unique_ptr<Node> mLogicOp;
	std::unique_ptr<Node> mLogicExpression;
};


// name = 'null null'
// mField -> name
// mValue = null null
class Expression : public Node
{
public:
	Expression(std::unique_ptr<Node> key, std::unique_ptr<Node> value) :
		Node(EXPR), mField(std::move(key)), mValue(std::move(value)) {}
	bool CreateQuery(Query& outQuery) const override;

private:
	std::unique_ptr<Node> mField;
	std::unique_ptr<Node> mValue;
};

class EmptyNode : public Node
{
public:
	EmptyNode(eSymbolName name) : Node(name) {}
	bool CreateQuery(Query& outQuery) const override;
};