#include "Node.h"

eSymbolName Node::GetName() const
{
	return mName;
}

const std::string Node::GetValue() const
{
	return  mValue;
}



bool Token::CreateQuery(Query& outQuery) const
{
	return true;
}

bool LogicExpression::CreateQuery(Query& outQuery) const
{
	return mExpression->CreateQuery(outQuery) && mMoreLogicExpression->CreateQuery(outQuery);
}

bool MoreLogicExpression::CreateQuery(Query& outQuery) const
{
	outQuery.AddLogicOperator(LOGIC_OPERATOR_TABLE.at(mLogicOp->GetValue()));
	return mLogicExpression->CreateQuery(outQuery);

}

bool Expression::CreateQuery(Query& outQuery) const
{
	auto it = FIELD_TABLE.find(mField->GetValue());
	if (it == FIELD_TABLE.end()) {
		return false;
	}
	outQuery.AddCondition(Condition(it->second, mValue->GetValue()));
	return true;
}

bool EmptyNode::CreateQuery(Query& outQuery) const
{
	return true;
}