#include "NodeFactory.h"

const std::unordered_map<eSymbolName, NodeFactory::NodeCreater> NodeFactory::NODE_CREATERS = {
	{LOGIC_EXPR, &NodeFactory::createLogicExpr},
	{MORE_LOGIC_EXPR, &NodeFactory::createMoreLogicExpr},
	{EXPR, &NodeFactory::createExpr},
};

std::unique_ptr<Node> NodeFactory::CreateNode(const eSymbolName lhs, const std::vector<eSymbolName>& rhs, std::stack<std::unique_ptr<Node>>& leftSubstring)
{
	// Epsion move creates EmptyNode 
	if (rhs.empty()) {
		return std::make_unique<EmptyNode>(EmptyNode(lhs));
	}
	return NODE_CREATERS.at(lhs)(lhs, rhs, leftSubstring);
}

std::unique_ptr<Node> NodeFactory::createLogicExpr(const eSymbolName lhs, const std::vector< eSymbolName>& rhs, std::stack<std::unique_ptr<Node>>& leftSubstring)
{
	std::unique_ptr<Node> moreLogicExpression = std::move(leftSubstring.top()); leftSubstring.pop();
	std::unique_ptr<Node> expression = std::move(leftSubstring.top()); leftSubstring.pop();
	return std::make_unique<LogicExpression>(LogicExpression(std::move(expression), std::move(moreLogicExpression)));
}
std::unique_ptr<Node> NodeFactory::createMoreLogicExpr(const eSymbolName lhs, const std::vector< eSymbolName>& rhs, std::stack<std::unique_ptr<Node>>& leftSubstring)
{
	std::unique_ptr<Node> logicExpression = std::move(leftSubstring.top()); leftSubstring.pop();
	std::unique_ptr<Node> logicalOperator = std::move(leftSubstring.top()); leftSubstring.pop();
	return std::make_unique<MoreLogicExpression>(MoreLogicExpression(std::move(logicalOperator), std::move(logicExpression)));
}
std::unique_ptr<Node> NodeFactory::createExpr(const eSymbolName lhs, const std::vector< eSymbolName>& rhs, std::stack<std::unique_ptr<Node>>& leftSubstring)
{
	std::unique_ptr<Node> value = std::move(leftSubstring.top()); leftSubstring.pop();
	leftSubstring.pop();
	std::unique_ptr<Node> field = std::move(leftSubstring.top()); leftSubstring.pop();
	return std::make_unique<Expression>(Expression(std::move(field), std::move(value)));
}
