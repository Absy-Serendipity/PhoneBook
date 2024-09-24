#pragma once


#include "Node.h"

class NodeFactory
{
public:
	static std::unique_ptr<Node> CreateNode(const eSymbolName, const std::vector<eSymbolName>& , std::stack<std::unique_ptr<Node>>&);

private:
	static std::unique_ptr<Node> createLogicExpr(const eSymbolName, const std::vector<eSymbolName>& , std::stack<std::unique_ptr<Node>>&);
	static std::unique_ptr<Node> createMoreLogicExpr(const eSymbolName, const std::vector<eSymbolName>& , std::stack<std::unique_ptr<Node>>&);
	static std::unique_ptr<Node> createExpr(const eSymbolName, const std::vector<eSymbolName>& , std::stack<std::unique_ptr<Node>>&);

private:
	// Table for static factory methods
	using NodeCreater = std::function<std::unique_ptr<Node> (const eSymbolName, const std::vector<eSymbolName>& , std::stack<std::unique_ptr<Node>>&)>;
	static const std::unordered_map<eSymbolName, NodeCreater> NODE_CREATERS;
};

