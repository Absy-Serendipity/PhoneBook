#include "Record.h"


void PrintTuple(const Tuple& tuple)
{
	std::cout << "Name: " << tuple.Name;
	std::cout << ", Address: " << tuple.Address;
	std::cout << ", PhoneNumber: " << tuple.PhoneNumber << std::endl;
}



Record::Record(const std::string& name, const std::string& address, const std::string& phoneNumber)
{
	mValues[NAME] = name;
	mValues[ADDRESS] = address;
	mValues[PHONE_NUMBER] = phoneNumber;
}

const std::string Record::GetValue(const eField keyType) const {
	return mValues.at(keyType);
}


void Record::SetByTuple(const Tuple& tuple)
{

	mValues[ADDRESS] = tuple.Address;
	mValues[NAME] = tuple.Name;
	mValues[PHONE_NUMBER] = tuple.PhoneNumber;
}

void Record::Setvalue(const eField keyType, const std::string value)
{
	mValues[keyType] = value;
}

void Record::SetValues(const std::unordered_map<eField, std::string>& updateValues)
{
	for (const auto& value : updateValues) {
		Setvalue(value.first, value.second);
	}
}


void Record::CopyTo(Tuple& outTuple) const
{
	outTuple.bDeleted = false;
	strcpy_s(outTuple.Name, mValues.at(NAME).c_str());
	strcpy_s(outTuple.Address, mValues.at(ADDRESS).c_str());
	strcpy_s(outTuple.PhoneNumber, mValues.at(PHONE_NUMBER).c_str());
}

std::unordered_map<eField, std::string>::iterator Record::begin() {
	return mValues.begin();
}

std::unordered_map<eField, std::string>::iterator Record::end() {
	return mValues.end();
}