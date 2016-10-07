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
Assert::IsTrue(Integer(num) == (num), L"Initialize Integer failed", LINE_INFO());\
}while(0)

			Assert::IsTrue(Integer() == 0, L"Initialize Integer failed", LINE_INFO());
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
		Assert::IsTrue(Integer(num1) op Integer(num2), L"Comparing failed", linfo); \
		Assert::IsTrue(Integer(num1) op (num2), L"Comparing failed", linfo); \
		Assert::IsTrue(Integer(num1) op (num2), L"Comparing failed", linfo); \
	} \
	else { \
		Assert::IsFalse(Integer(num1) op Integer(num2), L"Comparing failed", linfo);\
		Assert::IsFalse((num1) op Integer(num2), L"Comparing failed", linfo);\
		Assert::IsFalse(Integer(num1) op (num2), L"Comparing failed", linfo);\
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
			Logger::WriteMessage("In Method TestIntegerCompareOperator");

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

		template <typename T>
		void TestIntegerParseBase(T num, const std::string &str, int res, __LineInfo* pLineInfo) {
			Integer a(num), b;
			auto res2 = b.Parse(str);
			Assert::IsTrue(res == res2, L"Integer Parse Error", pLineInfo);
			Assert::IsTrue(b == a, L"Integer Parse Value Error", pLineInfo);
		}

		template <typename T>
		void TestIntegerParseRight(T num, const std::string &str, __LineInfo* pLineInfo) {
			TestIntegerParseBase(num, str, Number_Parse_OK, pLineInfo);
		}

		void TestIntegerParseWrong(const std::string &str, __LineInfo* pLineInfo) {
			TestIntegerParseBase(0, str, Number_Parse_Failed, pLineInfo);
		}

		TEST_METHOD(TestIntegerParse) {
			//2
			TestIntegerParseRight(0b0, "0b0", LINE_INFO());
			TestIntegerParseRight(0b0, "-0b0", LINE_INFO());
			TestIntegerParseRight(0b0, "+0b0", LINE_INFO());
			TestIntegerParseRight(0b0, "0B0", LINE_INFO());
			TestIntegerParseRight(0b110, "0b00000110", LINE_INFO());
			TestIntegerParseRight(0b11010010, "0b11010010", LINE_INFO());
			TestIntegerParseRight(-0b10001101, "-0b10001101", LINE_INFO());
			TestIntegerParseRight(0b10001100111011110111001101100101, "0B10001100111011110111001101100101", LINE_INFO());
			TestIntegerParseRight(0b0100110011101111011100110110010110001100111011110111001101100101,
				"0b0100110011101111011100110110010110001100111011110111001101100101", LINE_INFO());

			//8
			TestIntegerParseRight(00, "0o0", LINE_INFO());
			TestIntegerParseRight(01, "0O1", LINE_INFO());
			TestIntegerParseRight(-03, "-0o3", LINE_INFO());
			TestIntegerParseRight(05, "+0o5", LINE_INFO());
			TestIntegerParseRight(073, "0o000073", LINE_INFO());
			TestIntegerParseRight(0123045676, "0o123045676", LINE_INFO());
			TestIntegerParseRight(-0775310246, "-0O775310246", LINE_INFO());
			TestIntegerParseRight(012345673452671535530, "0o12345673452671535530", LINE_INFO());
			TestIntegerParseRight(-012345673452671537530, "-0o12345673452671537530", LINE_INFO());

			//10
			TestIntegerParseRight(0, "0", LINE_INFO());
			TestIntegerParseRight(1, "1", LINE_INFO());
			TestIntegerParseRight(2, "+2", LINE_INFO());
			TestIntegerParseRight(-9, "-9", LINE_INFO());
			TestIntegerParseRight(300753992, "300753992", LINE_INFO());
			TestIntegerParseRight(-10010, "-10010", LINE_INFO());
			TestIntegerParseRight(10086, "+10086", LINE_INFO());
			TestIntegerParseRight(2423569876418239237, "2423569876418239237", LINE_INFO());
			TestIntegerParseRight(-2423569876418239237, "-2423569876418239237", LINE_INFO());

			//16
			TestIntegerParseRight(0x0, "0x0", LINE_INFO());
			TestIntegerParseRight(0x1, "0x1", LINE_INFO());
			TestIntegerParseRight(0x5, "+0x5", LINE_INFO());
			TestIntegerParseRight(-0x8, "-0x8", LINE_INFO());
			TestIntegerParseRight(-0x7, "-0X7", LINE_INFO());
			TestIntegerParseRight(0xA8, "0x0000A8", LINE_INFO());
			TestIntegerParseRight(0x123456789ABCDEFD, "0x123456789abcdefd", LINE_INFO());
			TestIntegerParseRight(-0x123456789ABCDEFC, "-0x123456789abcdefc", LINE_INFO());
			TestIntegerParseRight(0xAA00CCDD00000000, "0XAA00CCDD00000000", LINE_INFO());
			TestIntegerParseRight(0xAA00CCDD00000000, "0xAA00ccdd00000000", LINE_INFO());
			//TestIntegerParseRight(, "", LINE_INFO());
			//TestIntegerParseRight(, "", LINE_INFO());


			TestIntegerParseWrong("", LINE_INFO());
			TestIntegerParseWrong("-", LINE_INFO());
			TestIntegerParseWrong("0123", LINE_INFO());
			TestIntegerParseWrong("0b1102", LINE_INFO());
			TestIntegerParseWrong("0o123467758", LINE_INFO());
			TestIntegerParseWrong("123743a", LINE_INFO());
			TestIntegerParseWrong("0x23c4dfaeg", LINE_INFO());
			TestIntegerParseWrong("0x-3df", LINE_INFO());
			//TestIntegerParseWrong("", LINE_INFO());
		}

		void TestIntegerToString(std::string str, std::string str10, std::string str16, __LineInfo* pLineInfo) {
			Integer a;
			Assert::AreEqual(Number_Parse_OK, a.Parse(str), L"Integer Parse error when test ToString()", pLineInfo);
			if (!str10.empty())
				Assert::AreEqual(str10, a.ToString10(), L"Integer error when convert to string 10", pLineInfo);
			if (!str16.empty())
				Assert::AreEqual(str16, a.ToString16(), L"Integer error when convert to string 10", pLineInfo);
		}

		TEST_METHOD(TestIntegerToString) {
			TestIntegerToString("0", "0", "0x0", LINE_INFO());
			TestIntegerToString("-0", "0", "0x0", LINE_INFO());

			TestIntegerToString("10", "10", "", LINE_INFO());
			TestIntegerToString("-20", "-20", "", LINE_INFO());
			TestIntegerToString("65536327680", "65536327680", "", LINE_INFO());
			TestIntegerToString("12345678909876543211357924681010987654321",
				"12345678909876543211357924681010987654321", "", LINE_INFO());
			TestIntegerToString("-12345678909876543211357924681010987654321",
				"-12345678909876543211357924681010987654321", "", LINE_INFO());

			TestIntegerToString("0x10", "16", "0x10", LINE_INFO());
			TestIntegerToString("-0XCC", "", "-0xCC", LINE_INFO());
			TestIntegerToString("-0X0000CC", "", "-0xCC", LINE_INFO());
			TestIntegerToString("0xabcdef", "", "0xABCDEF", LINE_INFO());
			TestIntegerToString("0X1237aaef", "", "0x1237AAEF", LINE_INFO());
			TestIntegerToString("0x123456789abcdeffeddc", "", "0x123456789ABCDEFFEDDC", LINE_INFO());
			TestIntegerToString("0x00000aacd", "", "0XAACD", LINE_INFO());
			//TestIntegerToString("", "", "", LINE_INFO());
			
		}

	};
}