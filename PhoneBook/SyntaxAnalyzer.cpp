#include "SyntaxAnalyzer.h"

std::unique_ptr<Node> SyntaxAnalyzer::Analyze(const LexicalAnalyzer& lexicalAnalyzer)
{
    mStates.push(0);

    while (true) {
        mNode = lexicalAnalyzer.GetToken();
        if (implementAction()) {
            return std::move(mLeftSubstring.top());
        }
    }
}

bool SyntaxAnalyzer::implementAction()
{
    int currentState = mStates.top();
    auto actionEntry = SLR_TABLE.at(currentState).find(mNode->GetName());
    if (actionEntry == SLR_TABLE.at(currentState).end()) {
        throw std::runtime_error(SYNTAX_ERROR_MESSAGE);
    }

    std::string action = actionEntry->second;
    std::string targetNumber = action.substr(1, action.length() - 1);
    switch (action[0])
    {
    case 's':
        shift(std::stoi(targetNumber));
        return false;
    case 'r':
        return reduce(std::stoi(targetNumber));
    case 'a':
        return true;
    default:
        goTo(std::stoi(action));
        return false;
    }
}


void SyntaxAnalyzer::goTo(const int targetState)
{
    mStates.push(targetState);
}

void SyntaxAnalyzer::shift(const int targetState)
{
    goTo(targetState);
    mLeftSubstring.push(std::move(mNode));
}


bool SyntaxAnalyzer::reduce(const int targetRuleNumber)
{
    eSymbolName lhs = LHS_LIST.at(targetRuleNumber);
    std::vector<eSymbolName> rhs = RHS_LIST.at(targetRuleNumber);


    for (int i = 0; i < rhs.size(); i++) {
        mStates.pop();
    }

    std::unique_ptr<Node> node = NodeFactory::CreateNode(lhs, rhs, mLeftSubstring);

    mLeftSubstring.push(std::move(node));


    goTo(std::stoi(SLR_TABLE[mStates.top()].at(lhs)));
    return implementAction();
}

