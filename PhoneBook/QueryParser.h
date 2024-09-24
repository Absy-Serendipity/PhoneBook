#pragma once
#include "SyntaxAnalyzer.h"


class QueryParser
{
public:
	QueryParser() {}
	std::unique_ptr<Query> Parse(const std::string& queryString);

private:
	SyntaxAnalyzer mSyntaxAnalyzer;
};

