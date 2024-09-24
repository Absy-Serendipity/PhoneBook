#pragma once
#include "Common.h"


class CommandView
{
public:
	const std::string& GetCommand() const;
	const std::string& GetName() const;
	const std::string& GetAddress() const;
	const std::string& GetPhoneNumber() const;
	const std::string& GetQueryString() const;
private:
	mutable std::string mInput;
};

