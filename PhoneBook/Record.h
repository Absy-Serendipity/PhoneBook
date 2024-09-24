#pragma once
#include "Condition.h"


typedef struct _Tuple
{
		bool bDeleted;
		char Name[MAX_NAME_SIZE + 1];
		char Address[MAX_ADDRESS_SIZE + 1];
		char PhoneNumber[PHONE_NUMBER_SIZE + 1];
} Tuple;

void PrintTuple(const Tuple&);

constexpr UINT TUPLE_SIZE = sizeof(Tuple);

class Record
{
public:
	Record() {}
	Record(const std::string&, const std::string&, const std::string&);

	const std::string GetValue(const eField) const;

	void SetByTuple(const Tuple&);
	void Setvalue(const eField , const std::string);
	void SetValues(const std::unordered_map<eField, std::string>&);

	void CopyTo(Tuple&) const;

	std::unordered_map<eField, std::string>::iterator begin();
	std::unordered_map<eField, std::string>::iterator end();

	
private:
	std::unordered_map<eField, std::string> mValues;
};


