#include "Query.h"

void Query::AddCondition(const Condition& condition)
{
	mConditions.push_back(condition);
}

void Query::AddLogicOperator(const eLogicOperator& logicOperator)
{
	mLogicOperators.push_back(logicOperator);
}

const std::vector<Condition>& Query::GetConditions() const
{
	return mConditions;
}

const std::vector<eLogicOperator>& Query::GetLogicOperators() const
{
	return mLogicOperators;
}

void Query::Print() const
{
	for (int index = 0; index < mConditions.size(); index++) {
		mConditions.at(index).Print();
		std::cout << LOGIC_OP_TO_STRING_TABLE.at(mLogicOperators.at(index)) << std::endl;
	}
}

bool Query::Evaluate(const Record& record) const
{
	bool bEvaluation = false;
	for (UINT index = 0; index < mConditions.size(); index++) {
		
		const std::string& compared = record.GetValue(mConditions[index].GetKeyType());
		const std::string& comparing = mConditions[index].GetValue();

		switch (mLogicOperators[index]) {
		case AND:
			bEvaluation &= compared == comparing;
			break;
		case OR:
			bEvaluation |= compared == comparing;
			break;
		default:
			break;
		}
	}
	return bEvaluation;
}