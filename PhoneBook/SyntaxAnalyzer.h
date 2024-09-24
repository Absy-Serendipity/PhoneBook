#pragma once
#include "LexicalAnalyzer.h"


const std::string SYNTAX_ERROR_MESSAGE = "Syntax analyzation error: Invalid syntax";
const std::vector<eSymbolName> LHS_LIST = {
		S,
		LOGIC_EXPR,
		MORE_LOGIC_EXPR,
		MORE_LOGIC_EXPR,
		EXPR,
};
const std::vector < std::vector<eSymbolName>> RHS_LIST = {
	{LOGIC_EXPR},
	{EXPR , MORE_LOGIC_EXPR},
	{LOGIC_OP , LOGIC_EXPR},
	{},
	{FIELD , EQUAL , VALUE},
};
const std::vector<std::unordered_map<eSymbolName, std::string>> SLR_TABLE = {
	{{FIELD, "s3"}, {LOGIC_EXPR, "1"}, {EXPR, "2"}},
	{{END_MARK, "acc"}},
	{{LOGIC_OP, "s5"}, {END_MARK, "r3"}, {MORE_LOGIC_EXPR, "4"}},
	{{EQUAL, "s6"}},
	{{END_MARK, "r1"}},
	{{FIELD, "s3"}, {LOGIC_EXPR, "7"}, {EXPR, "2"}},
	{{VALUE, "s8"}},
	{{END_MARK, "r2"}},
	{{LOGIC_OP, "r4"}, {END_MARK, "r4"}}

};

class SyntaxAnalyzer
{
public:
	SyntaxAnalyzer() {}
	std::unique_ptr<Node> Analyze(const LexicalAnalyzer& lexicalAnalyzer);

private:
	bool implementAction();
	void goTo(const int);
	void shift(const int);
	bool reduce(const int);

private:
	std::stack<int> mStates;
	std::stack<std::unique_ptr<Node>> mLeftSubstring;
	std::unique_ptr<Node> mNode;
};