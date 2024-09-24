#include "QueryParser.h"



std::unique_ptr<Query> QueryParser::Parse(const std::string& queryString)
{
    LexicalAnalyzer lexicalAnalyzer(queryString);

    std::unique_ptr<Node> syntaxTree = mSyntaxAnalyzer.Analyze(lexicalAnalyzer);
    Query query;
    query.AddLogicOperator(OR);
    syntaxTree->CreateQuery(query);
    
    return std::make_unique<Query>(query);
}
