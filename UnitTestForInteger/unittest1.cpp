#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Number;

namespace UnitTestForNumber
{		
	TEST_CLASS(UnitTest_Integer)
	{
	public:
	
		TEST_METHOD(Test_Integer_Initialize)
		{
			// TODO: 在此输入测试代码
			Integer((char)0);
			Integer((unsigned char)1);
			Integer((short)2);
			Integer((unsigned short)3);
			Integer((int)4);
			Integer((unsigned int)5);
			Integer((long)6);
			Integer((unsigned long)7);
			Integer((long long)8);
			Integer((unsigned long long)9);
		}

		TEST_METHOD(Test_Integer_Compare_Operator)
		{
			// TODO: 在此输入测试代码
#define TESTINTEGERCOMOP(num1, num2, op, res) do{\
if(res)\
	Assert::IsTrue(Integer(num1) op Integer(num2));\
else \
	Assert::IsFalse(Integer(num1) op Integer(num2));\
}while(0)

#define TESTINTEGEREQOP(num1, num2, res) do{\
		TESTINTEGERCOMOP(num1, num2, ==, res); \
		TESTINTEGERCOMOP(num1, num2, !=, !res); \
	}while(0)

			TESTINTEGEREQOP(0, 0,  true);
			TESTINTEGEREQOP(0, -0,  true);
			TESTINTEGEREQOP(10086, 10086, true);
			TESTINTEGEREQOP(-65536, -65536, true);
			TESTINTEGEREQOP(1234547576752345345, 1234547576752345345, true);
			TESTINTEGEREQOP(-1234547576752345345, -1234547576752345345, true);
			TESTINTEGEREQOP(1, 0, false);
			TESTINTEGEREQOP(3, -3, false);
			TESTINTEGEREQOP(1234547576752345345, 1234547576752345346, false);

			TESTINTEGERCOMOP(0, 1, < , true);
			TESTINTEGERCOMOP(0, 1, >, false);
			TESTINTEGERCOMOP(1, 0, <, false);
			TESTINTEGERCOMOP(1, 0, >, true);
			TESTINTEGERCOMOP(-1, 0, <, true);
			TESTINTEGERCOMOP(-1, 0, >, false);
			TESTINTEGERCOMOP(123456789098765, 123456789398765, <, true);
			TESTINTEGERCOMOP(123456789098765, 123456789398765, >, false);
			TESTINTEGERCOMOP(0, -0, <, false);
			TESTINTEGERCOMOP(0, -0, >, false);
			TESTINTEGERCOMOP(34, 34, <, false);
			TESTINTEGERCOMOP(34, 34, >, false);
			TESTINTEGERCOMOP(0x100C00F0, 0x100C00F0, <, false);
			TESTINTEGERCOMOP(0x100C00F0, 0x100C00F0, >, false);

			TESTINTEGERCOMOP(0, 1, <=, true);
			TESTINTEGERCOMOP(0, 1, >=, false);
			TESTINTEGERCOMOP(1, 0, <=, false);
			TESTINTEGERCOMOP(1, 0, >=, true);
			TESTINTEGERCOMOP(-1, 0, <=, true);
			TESTINTEGERCOMOP(-1, 0, >=, false);
			TESTINTEGERCOMOP(123456789098765, 123456789398765, <=, true);
			TESTINTEGERCOMOP(123456789098765, 123456789398765, >=, false);
			TESTINTEGERCOMOP(0, -0, <=, true);
			TESTINTEGERCOMOP(0, -0, >=, true);
			TESTINTEGERCOMOP(34, 34, <=, true);
			TESTINTEGERCOMOP(34, 34, >=, true);
			TESTINTEGERCOMOP(0x100C00F0, 0x100C00F0, <=, true);
			TESTINTEGERCOMOP(0x100C00F0, 0x100C00F0, >=, true);
		}

	};
}