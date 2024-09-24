#pragma once
#include "NodeFactory.h"

const std::string QUOTE_UNMACHED_ERROR_MESSAGE = "Lexical analyzation error: Quote unmatched";
const std::string UNEXPECTED_KEYWORD_ERROR_MESSAGE = "Lexical analyzation error: Unexpected keyword";

const std::unordered_set<char> WHITE_SPACES = {
	' ', '\t', '\n'
};

class LexicalAnalyzer
{
public:
	LexicalAnalyzer(const std::string& input) :mInput(input), mCurrentPosition(0) {}
	std::unique_ptr<Node> GetToken() const;
private:
	const std::string& mInput;
	mutable UINT mCurrentPosition;
};

