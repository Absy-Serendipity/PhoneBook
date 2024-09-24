#pragma once
#include "Repository.h"
#include "CommandView.h"
#include "QueryParser.h"

enum eCommandType {
	Insert, Search, Update, Remove, Exit, CommandTypeCount
};

class Controller
{
public:
	static Controller* GetInstance();
	void Run(Repository*, const CommandView*);
	
	Controller& operator=(const Controller&) = delete;
	Controller(const Controller&) = delete;

private:
	Controller() {}
	~Controller() {}


	std::unique_ptr<Record> getRecord() const;

	// Validators
	std::optional<eCommandType> convertToCommandType(const std::string&) const;
	bool verifyNewRecord(const Record&);
	bool verifyQuerySemantic(const Query&);

	// Field Validators
	bool verifyName(const std::string&);
	bool verifyAddress(const std::string&);
	bool verifyPhoneNumber(const std::string&);
	typedef bool (Controller::* Validator)(const std::string&);
	const Validator FIELD_VALIDATORS[FIELD_COUNT] = {
		&Controller::verifyName,&Controller::verifyAddress,
		&Controller::verifyPhoneNumber
	};
	
	// Command Handlers
	void insert();
	void search();
	void update();
	void remove();
	void terminate();
	typedef void (Controller::* CommandHandler)();
	const CommandHandler COMMAND_HANDLERS[CommandTypeCount] = {
		&Controller::insert,& Controller::search,& Controller::remove,
		&Controller::update, &Controller::terminate
	};
	
	
	

private:
	Repository* mRepository;
	const CommandView* mCommandView;
	QueryParser mQueryParser;
	bool mbEnd;
};