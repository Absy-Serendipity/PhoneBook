#include "Controller.h"


Controller* Controller::GetInstance()
{
	static Controller sController;
	return &sController;
}


void Controller::Run(Repository* repository, const CommandView* commandView)
{
	mRepository = repository;
	mCommandView = commandView;
	mbEnd = false;
	while (!mbEnd) 
	{
		const std::string& commandString = mCommandView->GetCommand();
		std::optional<eCommandType> oCommandType = convertToCommandType(commandString);
		if (!oCommandType.has_value()) 
		{
			std::cout << "Invalid Command: " << commandString << std::endl;
			continue;
		}
		try 
		{
			(this->*COMMAND_HANDLERS[oCommandType.value()])();
		}
		catch (std::runtime_error exception) 
		{
			std::cout << exception.what() << std::endl;
		}
	}
}

std::optional<eCommandType> Controller::convertToCommandType(const std::string& commandString) const
{
	try 
	{
		int commandType = std::stoi(commandString);
		if (commandType < Insert || commandType > Exit) 
		{
			return std::nullopt;
		}
		return static_cast<eCommandType>(commandType);
	}
	catch (const std::invalid_argument&) 
	{
		return std::nullopt;
	}
}

void Controller::insert()
{
	std::unique_ptr<Record> newRecord = getRecord();
	if (!verifyNewRecord(*newRecord)) 
	{
		std::cout << "Insertion Error: Invalid record" << std::endl;
		return;
	}
	mRepository->Add(std::move(newRecord));
}

void Controller::search()
{
	const std::string queryString = mCommandView->GetQueryString();
	std::unique_ptr<Query> query = mQueryParser.Parse(queryString);
	if (!verifyQuerySemantic(*query)) 
	{
		std::cout << "Search Error: Invalid Query" << std::endl;
		return;
	}
	mRepository->Load(std::move(query));
}

void Controller::update()
{
	// Get query string from user and validate it
	const std::string queryString = mCommandView->GetQueryString();
	std::unique_ptr<Query> query = mQueryParser.Parse(queryString);
	if (!verifyQuerySemantic(*query)) 
	{
		std::cout << "Update Error: Invalid Query" << std::endl;
		return;
	}


	// Get update record from user and validate it
	std::unique_ptr<Record> updateRecord = getRecord();
	std::unordered_map<eField, std::string> newEntries;
	for (const auto& updateValue : *updateRecord) 
	{
		if (updateValue.second.length() != 0) 
		{
			if (!(this->*FIELD_VALIDATORS[updateValue.first])(updateValue.second)) 
			{
				std::cout << "Update Error: Invalid " << FIELD_TO_STRING_TABLE.at(updateValue.first) << std::endl;
				return;
			}
			newEntries[updateValue.first] = updateValue.second;
		}
	}

	mRepository->Modify(std::move(query), newEntries);
}

void Controller::remove()
{
	const std::string queryString = mCommandView->GetQueryString();
	std::unique_ptr<Query> query = mQueryParser.Parse(queryString);
	if (!verifyQuerySemantic(*query)) 
	{
		std::cout << "Deletion Error: Invalid Query" << std::endl;
		return;
	}
	mRepository->Delete(std::move(query));
}

void Controller::terminate() 
{
	mbEnd = true;
}


bool Controller::verifyQuerySemantic(const Query& query) 
{
	for (const auto& condition : query.GetConditions()) 
	{
		if (!(this->*FIELD_VALIDATORS[condition.GetKeyType()])(condition.GetValue())) {
			return false;
		}
	}
	return true;
}

bool Controller::verifyNewRecord(const Record& record) 
{
	return (this->*FIELD_VALIDATORS[NAME])(record.GetValue(NAME))
		&& (this->*FIELD_VALIDATORS[ADDRESS])(record.GetValue(ADDRESS))
		&& (this->*FIELD_VALIDATORS[PHONE_NUMBER])(record.GetValue(PHONE_NUMBER));
}


bool Controller::verifyName(const std::string& TEST_NAME)
{
	return TEST_NAME.length() < MAX_NAME_SIZE;
}

bool Controller::verifyAddress(const std::string& TEST_ADDRESS)
{
	return TEST_ADDRESS.length() < MAX_ADDRESS_SIZE;
}

bool Controller::verifyPhoneNumber(const std::string& TEST_PHONE_NUMBER)
{
	if (TEST_PHONE_NUMBER.length() != PHONE_NUMBER_SIZE) 
	{
		return false;
	}
	for (char digit : TEST_PHONE_NUMBER) 
	{
		if (!std::isdigit(static_cast<unsigned char>(digit))) 
		{
			return false;
		}
	}
	return true;
}

std::unique_ptr<Record> Controller::getRecord() const
{
	const std::string name = mCommandView->GetName();
	const std::string address = mCommandView->GetAddress();
	const std::string phoneNumber = mCommandView->GetPhoneNumber();
	return std::make_unique<Record>(name, address, phoneNumber);
}