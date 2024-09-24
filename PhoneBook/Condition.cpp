#include "Condition.h"

const eField Condition::GetKeyType() const
{
	return mField;
}

const std::string& Condition::GetValue() const
{
	return mValue;
}
void Condition::Print() const
{
	std::cout << "Key: " << FIELD_TO_STRING_TABLE.at(GetKeyType()) << ", Value: " << GetValue() << std::endl;
}