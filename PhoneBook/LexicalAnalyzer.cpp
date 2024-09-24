#include "LexicalAnalyzer.h"



std::unique_ptr<Node> LexicalAnalyzer::GetToken() const
{
    if (mCurrentPosition >= mInput.length()) {
        return std::make_unique<Token>(Token(END_MARK));
    }

    if (WHITE_SPACES.find(mInput[mCurrentPosition]) != WHITE_SPACES.end()) {
        mCurrentPosition++;
        return GetToken();
    }

    if (mInput[mCurrentPosition] == '=') {
        mCurrentPosition++;
        return std::make_unique<Token>(Token(EQUAL));
    }

    std::string value;
    if (mInput[mCurrentPosition] == '\'' || mInput[mCurrentPosition] == '"') {
        char quote = mInput[mCurrentPosition];
        while (mInput[++mCurrentPosition] != quote) {
            if (mCurrentPosition >= mInput.length()) {
                throw std::runtime_error(QUOTE_UNMACHED_ERROR_MESSAGE);
            }
            value += mInput[mCurrentPosition];
        }
        mCurrentPosition++;
        return std::make_unique<Token>(Token(VALUE, value));
    }

    while (std::isalnum(mInput[mCurrentPosition]) || mInput[mCurrentPosition] == '_') {
        value += std::tolower(mInput[mCurrentPosition++]);
    }

    auto it = SYMBOL_NAME_TABLE.find(value);
    if (it != SYMBOL_NAME_TABLE.end()) {
        return std::make_unique<Token>(Token(it->second, value));
    }

    throw std::runtime_error(UNEXPECTED_KEYWORD_ERROR_MESSAGE);
}