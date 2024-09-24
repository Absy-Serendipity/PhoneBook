
#include "pch.h"
#include "CppUnitTest.h"
#include "Controller.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
const std::string TEST_NAME = "aiden";
const std::string TEST_ADDRESS = "Seoul";
const std::string TEST_PHONE_NUMBER = "12341234";

namespace record_test {
	TEST_CLASS(RecordTest)
	{
	private:
		
	public:
		TEST_METHOD(ConstructorTest)
		{
			Record record(TEST_NAME, TEST_ADDRESS, TEST_PHONE_NUMBER);

			Assert::AreEqual(TEST_NAME, record.GetValue(NAME));
			Assert::AreEqual(TEST_ADDRESS, record.GetValue(ADDRESS));
			Assert::AreEqual(TEST_PHONE_NUMBER, record.GetValue(PHONE_NUMBER));
		}

		TEST_METHOD(SetByTupleTest)
		{
			Tuple testTuple;
			strcpy_s(testTuple.Address, TEST_ADDRESS.c_str());
			strcpy_s(testTuple.Name, TEST_NAME.c_str());
			strcpy_s(testTuple.PhoneNumber, TEST_PHONE_NUMBER.c_str());

			Record record;
			record.SetByTuple(testTuple);

			Assert::AreEqual(TEST_NAME, record.GetValue(NAME));
			Assert::AreEqual(TEST_ADDRESS, record.GetValue(ADDRESS));
			Assert::AreEqual(TEST_PHONE_NUMBER, record.GetValue(PHONE_NUMBER));
		}

		TEST_METHOD(CopyTo)
		{
			Record record(TEST_NAME, TEST_ADDRESS, TEST_PHONE_NUMBER);
			Tuple testTuple;
			record.CopyTo(testTuple);

			Assert::AreEqual(testTuple.Name, TEST_NAME.c_str());
			Assert::AreEqual(testTuple.Address, TEST_ADDRESS.c_str());
			Assert::AreEqual(testTuple.PhoneNumber, TEST_PHONE_NUMBER.c_str());
		}

		
	};

}



namespace query_parser_test {
	TEST_CLASS(QueryParserTest)
	{
	
	private:
		QueryParser mQueryParser;

	public:
		TEST_METHOD(ParseTest)
		{
			std::unique_ptr<Query> query = mQueryParser.Parse("name = 'nullnull' and address = \"Seoul\"");
			
			const auto& conditions = query->GetConditions();
			const auto& logicOperators = query->GetLogicOperators();

			Assert::IsTrue(conditions.at(0).GetKeyType() == NAME);
			Assert::AreEqual(std::string("nullnull"), conditions.at(0).GetValue());
			Assert::IsTrue(logicOperators.at(0) == OR);

			Assert::IsTrue(conditions.at(1).GetKeyType() == ADDRESS);
			Assert::AreEqual(std::string("Seoul"), conditions.at(1).GetValue());
			Assert::IsTrue(logicOperators.at(1)== AND);
		}


		TEST_METHOD(EvaluateTest)
		{
			Record record(TEST_NAME, TEST_ADDRESS, TEST_PHONE_NUMBER);

			// (SAME NAME) and (SAME ADDRESS) -> true
			Query query;
			query.AddCondition(Condition(NAME, TEST_NAME));
			query.AddCondition(Condition(ADDRESS, TEST_ADDRESS));
			query.AddLogicOperator(OR);
			query.AddLogicOperator(AND);

			Assert::IsTrue(query.Evaluate(record));

			
			const std::string DIFFERENT_NAME = "NULL";
			// (DIFFERENT NAME) and (SAME ADDRESS) -> false
			query = Query();
			query.AddCondition(Condition(NAME, DIFFERENT_NAME));
			query.AddCondition(Condition(ADDRESS, TEST_ADDRESS));
			query.AddLogicOperator(OR);
			query.AddLogicOperator(AND);

			Assert::IsFalse(query.Evaluate(record));


			// (DIFFERENT NAME) or (SAME ADDRESS) -> true
			query = Query();
			query.AddCondition(Condition(NAME, DIFFERENT_NAME));
			query.AddCondition(Condition(ADDRESS, TEST_ADDRESS));
			query.AddLogicOperator(OR);
			query.AddLogicOperator(OR);
			Assert::IsTrue(query.Evaluate(record));
		}

		TEST_METHOD(LexicalErrorTest)
		{
			try {
				// 'nullnull -> quote is not closed
				std::unique_ptr<Query> token = mQueryParser.Parse("name = 'nullnull and address = \"Seoul\"");
				Assert::Fail();
			}
			catch (std::runtime_error e) {
				Assert::AreEqual(QUOTE_UNMACHED_ERROR_MESSAGE, std::string(e.what()));
			}

			try {
				// keyword age doesn't exist
				std::unique_ptr<Query> token = mQueryParser.Parse("age = '31' and address = Seoul\"");
				Assert::Fail();
			}
			catch (std::runtime_error e) {
				Assert::AreEqual(UNEXPECTED_KEYWORD_ERROR_MESSAGE, std::string(e.what()));
			}
		}

		TEST_METHOD(SyntaxErrorTest)
		{
			try {
				// logical operator such as 'and'/'or' doesn't exist
				std::unique_ptr<Query> query = mQueryParser.Parse("name = 'nullnull' address = \"Seoul\"");
				Assert::Fail();
			}
			catch (std::runtime_error e) {
				Assert::AreEqual(SYNTAX_ERROR_MESSAGE, std::string(e.what()));
			}

			try {
				// only keyword exsits
				std::unique_ptr<Query> query = mQueryParser.Parse("name");
				Assert::Fail();
			}
			catch (std::runtime_error e) {
				Assert::AreEqual(SYNTAX_ERROR_MESSAGE, std::string(e.what()));
			}

			try {
				// only value exsits
				std::unique_ptr<Query> query = mQueryParser.Parse("'nullnull'");
				Assert::Fail();
			}
			catch (std::runtime_error e) {
				Assert::AreEqual(SYNTAX_ERROR_MESSAGE, std::string(e.what()));
			}
		}
	};
}
