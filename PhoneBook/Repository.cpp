#include "Repository.h"


// Singleton Pattern
Repository* Repository::sRepository = new Repository;

Repository* Repository::GetInstance()
{
    return sRepository;
}

// Observer Pattern
void Repository::NotifyAll() const
{
    for (const auto& observer : mObservers) {
        observer->Update(mLoadedTuples);
    }
}

Repository::Repository()
{
    mDataFile = std::fstream(DATA_FILE_NAME, std::ios::in | std::ios::out | std::ios::binary);
}

Repository::~Repository()
{
    mDataFile.close();
}


UINT Repository::Add(const std::unique_ptr<Record> newRecord) 
{
    // Check duplciate phone number of new record
    if (checkDuplicatePhoneNumber(newRecord->GetValue(PHONE_NUMBER).c_str()))
    {
        // if duplicate, return UNIT_MAX(Sentinel value)
        return UINT_MAX;
    }
    
    // Most recently deleted tuple's position will be returned
    // If no tuple is deleted, (number of tuples in file) + 1 is returend, to push back new record 
    UINT insertedPosition = getInsertionPosition();

    std::cout << insertedPosition << std::endl;

    Tuple tuple;
    newRecord->CopyTo(tuple);
    mDataFile.seekp(TUPLE_SIZE * insertedPosition, std::ios::beg);
    mDataFile.write(reinterpret_cast<const char*>(&tuple), TUPLE_SIZE);
    std::cout << "After insertion: " << getTupleCount() << std::endl;
    return insertedPosition;
}

UINT Repository::Load(const std::unique_ptr<Query> query)
{

    UINT tupleCount = getTupleCount();
    Tuple tuple;
    Record record;

    mLoadedTuples.clear();
    // Loop for number of stored tuples
    for (UINT position = 0; position < tupleCount; ++position) {
        // Get current position's tuple
        mDataFile.seekg(TUPLE_SIZE * position, std::ios::beg);
        mDataFile.read(reinterpret_cast<char*>(&tuple), TUPLE_SIZE);

        // If tuple is deleted, do nothing
        if (tuple.bDeleted) {
            continue;
        }

        // Set record by tuple
        record.SetByTuple(tuple);
        // Evalute query for record
        // If evaluation is true, append tuple
        if (query->Evaluate(record)) {
            mLoadedTuples.push_back(tuple);
        }
    }
    // Inform observers of retrieved tuples
    NotifyAll();

    // return number of retrieved tuples
    return (UINT)mLoadedTuples.size();
}



UINT Repository::Delete(const std::unique_ptr<Query> query)
{
    // Open file where postions of deleted tuples are stored
    std::ofstream deletedPositionFile(DELETED_POSITION_FILE_NAME, std::ios::binary);

    bool deletedFlag = true;
    UINT deletionCount = 0;
    Tuple tuple;
    Record record;
    UINT tupleCount = getTupleCount();

    // Loop for number of stored tuples
    for (UINT position = 0; position < tupleCount; ++position) {
        // Get current position's tuple
        mDataFile.seekg(TUPLE_SIZE * position, std::ios::beg);
        mDataFile.read(reinterpret_cast<char*>(&tuple), TUPLE_SIZE);
        
        // If tuple is deleted, do nothing
        if (tuple.bDeleted) {
            continue;
        }

        // Set record by tuple
        record.SetByTuple(tuple);

        // Evalute query for record
        // If evaluation is true
        // 1. Set tuple's deletion flag within data file
        // 2. Append current postion to file where postions of deleted tuples are stored
        // 3. Add up number of deleted tuples
        if (query->Evaluate(record)) {
            // 1
            mDataFile.seekp(TUPLE_SIZE * position, std::ios::beg);
            mDataFile.write(reinterpret_cast<char*>(&deletedFlag), sizeof(bool));
            std::cout << position << std::endl;
            // 2
            deletedPositionFile.seekp(0, std::ios::end);
            deletedPositionFile.write(reinterpret_cast<char*>(&position), sizeof(UINT));
            
            // 3
            ++deletionCount;
        }
    }

    // Close file where postions of deleted tuples are stored
    deletedPositionFile.close();
    return deletionCount;
}



UINT Repository::Modify(const std::unique_ptr<Query> query, const std::unordered_map<eField, std::string>& updateValues){
    auto itPhoneNumber = updateValues.find(PHONE_NUMBER);
    UINT updateCount = 0;
    // If one of update values is phone number,
    if (itPhoneNumber != updateValues.end()) {
        // Check duplciate phone number
        if (checkDuplicatePhoneNumber(itPhoneNumber->second.c_str()))
        {
            return updateCount;
        }
    }

    
    UINT tupleCount = getTupleCount();
    
    Tuple tuple;
    Record record;
    mDataFile.seekg(0, std::ios::beg);

    // Loop for number of stored tuples
    for (UINT position = 0; position < tupleCount; ++position) {
        // Get current position's tuple
        mDataFile.seekg(TUPLE_SIZE * position, std::ios::beg);
        mDataFile.read(reinterpret_cast<char*>(&tuple), TUPLE_SIZE);

        // Never check deleted tuples, which leads to instruction branch
        // Assuming most of deleted tuples are overwritten by new tuples
        
        // Set record by tuple
        record.SetByTuple(tuple);

        // Evalute query for record
        // If evaluation is true
        // 1. Set record with update values
        // 2. Write back
        // 3. Add up number of updated tuples
        if (query->Evaluate(record)) {
            // 1
            record.SetValues(updateValues);
            record.CopyTo(tuple);

            // 2
            mDataFile.seekp(TUPLE_SIZE * position, std::ios::beg);
            mDataFile.write(reinterpret_cast<const char*>(&tuple), TUPLE_SIZE);
            
            // 3
            ++updateCount;

            // For modification of phone number,
            // only first found record is modified to keep uniqueness
            if (itPhoneNumber != updateValues.end()) {
                return updateCount;
            }
        }
    }
    return updateCount;
}




bool Repository::checkDuplicatePhoneNumber(const char phoneNumber[PHONE_NUMBER_SIZE + 1])
{
    UINT tupleCout = getTupleCount();
    Tuple tuple;
    for (UINT position = 0; position < tupleCout; ++position) {
        mDataFile.seekg(TUPLE_SIZE * position, std::ios::beg);
        mDataFile.read(reinterpret_cast<char*>(&tuple), TUPLE_SIZE);
        if (tuple.bDeleted) {
            continue;
        }
        if (memcmp(tuple.PhoneNumber, phoneNumber, PHONE_NUMBER_SIZE) == 0) {
            return true;
        }
    }
    return false;
}


UINT Repository::getInsertionPosition()
{
    std::streamoff fileSize;
    UINT insertedPosition;
    std::vector<UINT> positions;
    
    // Read deleted postions from deleted postion file
    // Pop out MOST RECENTLY deleted position
    {
        std::ifstream inputStream(DELETED_POSITION_FILE_NAME, std::ios::binary);
        inputStream.seekg(0, std::ios::end);
        fileSize = inputStream.tellg();

        // If there is no deleted position
        // return ( number of tuples in file ) + 1
        if (fileSize == 0) {
            inputStream.close();
            mDataFile.seekg(0, std::ios::end);
            return static_cast<UINT>(mDataFile.tellg() / TUPLE_SIZE);
        }

        positions = std::vector<UINT>(fileSize / sizeof(UINT));

        inputStream.seekg(0, std::ios::beg);
        inputStream.read(reinterpret_cast<char*>(positions.data()), fileSize);  
        insertedPosition = positions.back(); positions.pop_back();

        inputStream.close();
    }
    
    // Write back deleted positions to deleted postion file, except MOST RECENTLY deleted position
    {
        std::ofstream outputStream(DELETED_POSITION_FILE_NAME, std::ios::binary | std::ios::trunc);
        outputStream.write(reinterpret_cast<char*>(positions.data()), fileSize - sizeof(UINT));
        outputStream.close();
    }

    // return MOST RECENTLY deleted position
    return insertedPosition;
}

UINT Repository::getTupleCount()
{
    // seek to end of file, to get size of file
    mDataFile.seekg(0, std::ios::end);

    // Data file only stroes tuples
    // so tha, number of stored tuples = file size / tuple size
    return static_cast<UINT>(mDataFile.tellg() / TUPLE_SIZE);
}