#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Number;

namespace UnitTestForNumber
{		
	BEGIN_TEST_MODULE_ATTRIBUTE()
		TEST_MODULE_ATTRIBUTE(L"Date", L"2010/6/12")
	END_TEST_MODULE_ATTRIBUTE()

	TEST_CLASS(UnitTestForInteger)
	{
	public:
		UnitTestForInteger() {
			Logger::WriteMessage("In UnitTestForInteger.");
		}

		~UnitTestForInteger() {
			Logger::WriteMessage("Leave UnitTestForInteger.");
		}

		TEST_METHOD(TestIntegerInitialize)
		{
			// TODO: 在此输入测试代码
			Logger::WriteMessage("In Method TestIntegerInitialize.");

#define TEST_INTEGER_INITIALIZE(num) do{\
Assert::IsTrue(Integer(num) == (num));\
}while(0)

			TEST_INTEGER_INITIALIZE((char)0);
			TEST_INTEGER_INITIALIZE((unsigned char)1);
			TEST_INTEGER_INITIALIZE((short)2);
			TEST_INTEGER_INITIALIZE((unsigned short)3);
			TEST_INTEGER_INITIALIZE((int)4);
			TEST_INTEGER_INITIALIZE((unsigned int)5);
			TEST_INTEGER_INITIALIZE((long)6);
			TEST_INTEGER_INITIALIZE((unsigned long)7);
			TEST_INTEGER_INITIALIZE((long long)8);
			TEST_INTEGER_INITIALIZE((unsigned long long)9);
		}

		TEST_METHOD(TestIntegerCompareOperator)
		{
			// TODO: 在此输入测试代码
			Logger::WriteMessage("In Method TestIntegerCompareOperator.");

#define TEST_INTEGER_COM_BASE(num1, num2, op, res) do{\
if(res)\
	Assert::IsTrue((Integer(num1)) op (Integer(num2)));\
else \
	Assert::IsFalse(Integer(num1) op Integer(num2));\
}while(0)

			//num1 <= num2, eq = true iff num1 == num2
#define TEST_INTEGER_COMPARE_BASE2(num1, num2, eq) do{\
TEST_INTEGER_COM_BASE(num1, num2, ==, eq); \
TEST_INTEGER_COM_BASE(num1, num2, !=, !eq); \
TEST_INTEGER_COM_BASE(num2, num1, ==, eq); \
TEST_INTEGER_COM_BASE(num2, num1, !=, !eq); \
TEST_INTEGER_COM_BASE(num1, num2, <, !eq);\
TEST_INTEGER_COM_BASE(num2, num1, <, false);\
TEST_INTEGER_COM_BASE(num1, num2, >, false);\
TEST_INTEGER_COM_BASE(num2, num1, >, !eq);\
TEST_INTEGER_COM_BASE(num1, num2, <=, true);\
TEST_INTEGER_COM_BASE(num2, num1, <=, eq);\
TEST_INTEGER_COM_BASE(num1, num2, >=, eq);\
TEST_INTEGER_COM_BASE(num2, num1, >=, true);\
}while(0)

#define TEST_INTEGER_COMPARE_NUM(num1, num2) do{ \
if (num1 < num2) \
TEST_INTEGER_COMPARE_BASE2(num1, num2, false); \
else if (num1 > num2) \
TEST_INTEGER_COMPARE_BASE2(num2, num1, false); \
else \
TEST_INTEGER_COMPARE_BASE2(num1, num2, true); \
}while(0)

			//two equal number
			TEST_INTEGER_COMPARE_NUM(0, 0);
			TEST_INTEGER_COMPARE_NUM(0, -0);
			TEST_INTEGER_COMPARE_NUM(1, 1);
			TEST_INTEGER_COMPARE_NUM(-1, -1);
			TEST_INTEGER_COMPARE_NUM(10086, 10086);
			TEST_INTEGER_COMPARE_NUM(-65536, -65536);
			TEST_INTEGER_COMPARE_NUM(1234547576752345345, 1234547576752345345);
			TEST_INTEGER_COMPARE_NUM((int64)-1234547576752345345, (int64)-1234547576752345345);

			//two different number
			TEST_INTEGER_COMPARE_NUM(0, 1);
			TEST_INTEGER_COMPARE_NUM(0, -1);
			TEST_INTEGER_COMPARE_NUM(-12323, 5);
			TEST_INTEGER_COMPARE_NUM(13579, 246810);
			TEST_INTEGER_COMPARE_NUM(-13579, -2345);
			TEST_INTEGER_COMPARE_NUM((int64)1234547576752345345, (int64)1234547576752345346);
			TEST_INTEGER_COMPARE_NUM((int64)-1234547576752345345, (int64)-1234547576752345346);
		}

	};
}