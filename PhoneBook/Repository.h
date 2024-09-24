#pragma once
#include "Query.h"
#include "Record.h"
#include "Observable.h"


const std::string DATA_FILE_NAME = "phonebook.dat";
const std::string DELETED_POSITION_FILE_NAME = "deletedData.dat";

class Repository : public Observable
{
public:
	UINT Add(const std::unique_ptr<Record>) ;
	UINT Delete(const std::unique_ptr<Query>);
	UINT Load(const std::unique_ptr<Query>) ;
	UINT Modify(const std::unique_ptr<Query>, const std::unordered_map<eField, std::string>&);

	// Singleton
	Repository(const Repository&) = delete;
	Repository& operator=(const Repository&) = delete;
	static Repository* GetInstance();
	
	// Observable
	void NotifyAll() const override;
private:
	// Singleton management
	Repository();
	~Repository();


	bool checkDuplicatePhoneNumber(const char[PHONE_NUMBER_SIZE + 1]);
	UINT getInsertionPosition();
	UINT getTupleCount();

private:
	std::fstream mDataFile;
	std::vector<Tuple> mLoadedTuples;

	// Singleton Object
	static Repository* sRepository;
};