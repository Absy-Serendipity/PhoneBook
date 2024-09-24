#pragma once
#include "Condition.h"
#include "Record.h"

class Query
{
public:
	Query() {}
	void AddCondition(const Condition&);
	void AddLogicOperator(const eLogicOperator&);
	const std::vector<Condition>& GetConditions() const;
	const std::vector<eLogicOperator>& GetLogicOperators() const;
	void Print() const;
	bool Evaluate(const Record&) const;

private:
	std::vector<Condition> mConditions;
	std::vector<eLogicOperator> mLogicOperators;
};


