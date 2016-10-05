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
			Logger::WriteMessage("Leave UnitTestForInteger.\n");
		}

		TEST_METHOD(TestIntegerInitialize)
		{
			// TODO: 在此输入测试代码
			Logger::WriteMessage("In Method TestIntegerInitialize.\n");

#define TEST_INTEGER_INITIALIZE(num) do{\
Assert::IsTrue(Integer(num) == (num), L"Initialize Integer failed.\n", LINE_INFO());\
}while(0)

			Assert::IsTrue(Integer() == 0, L"Initialize Integer failed.\n", LINE_INFO());
			TEST_INTEGER_INITIALIZE((char)0);
			TEST_INTEGER_INITIALIZE((unsigned char)1);
			TEST_INTEGER_INITIALIZE((short)65535);
			TEST_INTEGER_INITIALIZE((unsigned short)32767);
			TEST_INTEGER_INITIALIZE((int)4);
			TEST_INTEGER_INITIALIZE((unsigned int)5);
			TEST_INTEGER_INITIALIZE((long)-0x60000000);
			TEST_INTEGER_INITIALIZE((unsigned long)7);
			TEST_INTEGER_INITIALIZE((signed long long)-0x800700060005000);
			TEST_INTEGER_INITIALIZE((unsigned long long)0xfff0fff0fff0fff0);
		}

#define TEST_INTEGER_COM_BASE(num1, num2, op, res, linfo) do{ \
	if(res) { \
		Assert::IsTrue(Integer(num1) op Integer(num2), L"Comparing failed.\n", linfo); \
		Assert::IsTrue(Integer(num1) op (num2), L"Comparing failed.\n", linfo); \
		Assert::IsTrue(Integer(num1) op (num2), L"Comparing failed.\n", linfo); \
	} \
	else { \
		Assert::IsFalse(Integer(num1) op Integer(num2), L"Comparing failed.\n", linfo);\
		Assert::IsFalse((num1) op Integer(num2), L"Comparing failed.\n", linfo);\
		Assert::IsFalse(Integer(num1) op (num2), L"Comparing failed.\n", linfo);\
	}\
}while(0)

		//num1 <= num2, eq = true iff num1 == num2
		template<typename U, typename V> 
		void TestIntegerCompareBase2(U num1, V num2, bool eq, const __LineInfo* pLineInfo = NULL) {
			TEST_INTEGER_COM_BASE(num1, num2, == , eq, pLineInfo);
			TEST_INTEGER_COM_BASE(num1, num2, != , !eq, pLineInfo);
			TEST_INTEGER_COM_BASE(num2, num1, == , eq, pLineInfo);
			TEST_INTEGER_COM_BASE(num2, num1, != , !eq, pLineInfo);
			TEST_INTEGER_COM_BASE(num1, num2, <, !eq, pLineInfo);
			TEST_INTEGER_COM_BASE(num2, num1, <, false, pLineInfo);
			TEST_INTEGER_COM_BASE(num1, num2, >, false, pLineInfo);
			TEST_INTEGER_COM_BASE(num2, num1, >, !eq, pLineInfo);
			TEST_INTEGER_COM_BASE(num1, num2, <= , true, pLineInfo);
			TEST_INTEGER_COM_BASE(num2, num1, <= , eq, pLineInfo);
			TEST_INTEGER_COM_BASE(num1, num2, >= , eq, pLineInfo);
			TEST_INTEGER_COM_BASE(num2, num1, >= , true, pLineInfo);
		}

		template<typename U, typename V>
		void TestIntegerCompareNumber(U num1, V num2, const __LineInfo* pLineInfo = NULL) {
			if (num1 < num2)
				TestIntegerCompareBase2(num1, num2, false, pLineInfo);
			else if (num1 > num2)
				TestIntegerCompareBase2(num2, num1, false, pLineInfo);
			else
				TestIntegerCompareBase2(num1, num2, true, pLineInfo);
		}

		TEST_METHOD(TestIntegerCompareOperator)
		{
			// TODO: 在此输入测试代码
			Logger::WriteMessage("In Method TestIntegerCompareOperator.\n");

			//two equal numbers
			TestIntegerCompareNumber(0, 0, LINE_INFO());
			TestIntegerCompareNumber(0, -0, LINE_INFO());
			TestIntegerCompareNumber(1, 1, LINE_INFO());
			TestIntegerCompareNumber(-1, -1, LINE_INFO());
			TestIntegerCompareNumber(10086, 10086, LINE_INFO());
			TestIntegerCompareNumber(-65536, -65536, LINE_INFO());
			TestIntegerCompareNumber(0xDF00, 0xDF00, LINE_INFO());
			TestIntegerCompareNumber(-0xDC00, -0xDC00, LINE_INFO());
			TestIntegerCompareNumber((int64)0xDF0000000000, (int64)0xDF0000000000, LINE_INFO());
			TestIntegerCompareNumber((int64)-0xDC0000000000, (int64)-0xDC0000000000, LINE_INFO());
			TestIntegerCompareNumber(1234547576752345345, 1234547576752345345, LINE_INFO());
			TestIntegerCompareNumber((int64)-1234547576752345345, (int64)-1234547576752345345, LINE_INFO());

			//two different numbers
			TestIntegerCompareNumber(0, 1, LINE_INFO());
			TestIntegerCompareNumber(0, -1, LINE_INFO());
			TestIntegerCompareNumber(-12323, 5, LINE_INFO());
			TestIntegerCompareNumber(13579, 246810, LINE_INFO());
			TestIntegerCompareNumber(-13579, -2345, LINE_INFO());
			TestIntegerCompareNumber(0xCF00, 0xDF00, LINE_INFO());
			TestIntegerCompareNumber(-0xDC00, -0xAE00, LINE_INFO());
			TestIntegerCompareNumber(-0xDC00, 0xA0, LINE_INFO());
			TestIntegerCompareNumber(0xCF0000000000, 0xDF0000000000, LINE_INFO());
			TestIntegerCompareNumber(-0xDC0000000000, -0xAE0000000000, LINE_INFO());
			TestIntegerCompareNumber(-0xDC0000000000, 0xA000000000, LINE_INFO());
			TestIntegerCompareNumber((int64)1234547576752345345, (int64)1234547576752345346, LINE_INFO());
			TestIntegerCompareNumber((int64)-1234547576752345345, (int64)-1234547576752345346, LINE_INFO());
		}

	};
}