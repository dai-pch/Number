#include "catch.hpp"

#include <random>
#include <functional>
#include "../Number/Real.h"
using namespace Number;

static int real_test_num = 50;

Real RandomRealNumber() {
	static std::default_random_engine generator((unsigned)time(NULL));
	static std::uniform_int_distribution<save_type> distribution, size_d(1, 1024), sig(0, 1);
	static auto dice = std::bind(distribution, generator);
	int size = size_d(generator);
	std::vector<save_type> vec(size);
	for (auto& num : vec) {
		num = dice();
	}
	if (vec.back() == 0)
		++vec.back();
	return Real(vec, sig(generator) ? 1 : -1);
}

TEST_CASE("Test initialize for Real", "[Initialize][Real]") {
	REQUIRE(Real() == 0);
	Real(1.0);
	Real((unsigned char)120);
	Real((unsigned int)3);
	Real((unsigned long)65536);
	Real((char)-105);
	Real((int)-236);
	Real((long)-657386);
	Real((float)3.1415926e10);
	Real((double)-2.718281828e6);
	Real((double)308273954e-200);
}

#define TEST_COM_BASE(TYPE, num1, num2, op, res) do{ \
	if(res) { \
		REQUIRE(TYPE(num1) op TYPE(num2)); \
		REQUIRE(TYPE(num1) op (num2)); \
		REQUIRE((num1) op TYPE(num2)); \
	} \
	else { \
		REQUIRE_FALSE(TYPE(num1) op TYPE(num2));\
		REQUIRE_FALSE((num1) op TYPE(num2));\
		REQUIRE_FALSE(TYPE(num1) op (num2));\
	}\
}while(0)

//num1 <= num2, eq = true iff. num1 == num2
template<typename Type, typename U, typename V>
void TestCompareBase2(U num1, V num2, bool eq) {
	TEST_COM_BASE(Type, num1, num2, == , eq);
	TEST_COM_BASE(Type, num1, num2, != , !eq);
	TEST_COM_BASE(Type, num2, num1, == , eq);
	TEST_COM_BASE(Type, num2, num1, != , !eq);
	TEST_COM_BASE(Type, num1, num2, <, !eq);
	TEST_COM_BASE(Type, num2, num1, <, false);
	TEST_COM_BASE(Type, num1, num2, >, false);
	TEST_COM_BASE(Type, num2, num1, >, !eq);
	TEST_COM_BASE(Type, num1, num2, <= , true);
	TEST_COM_BASE(Type, num2, num1, <= , eq);
	TEST_COM_BASE(Type, num1, num2, >= , eq);
	TEST_COM_BASE(Type, num2, num1, >= , true);
}

template<typename Type, typename U, typename V>
void TestCompareNumber(U num1, V num2) {
	if (num1 < num2)
		TestCompareBase2<Type>(num1, num2, false);
	else if (num1 > num2)
		TestCompareBase2<Type>(num2, num1, false);
	else
		TestCompareBase2<Type>(num1, num2, true);
}

TEST_CASE("Test compare operator for Real", "[Compare][Real]") {
	SECTION("Equal numbers.") {
		double test_number_1[] = { 0, 1, -1, 10086, -65536, 0xDF00, -0xDC00,
			0xDF00000, -0xDC00000, 1454767345, -1454767345, 0.0,
			271.8281828354, -65.336 };

		for (auto ele : test_number_1) {
			TestCompareNumber<Real>(ele, ele);
		}
	}

	SECTION("Nonequal numbers.") {
		std::pair<double, double> test_number_2[] = { { 0,1 },{ 0, -1 },
		{ -12323, 5 },{ 13579, 246810 },{ -13579, -2345 },{ 0xCF00, 0xDF00 },
		{ 467539025, 467539026 },{ -467539025, -467539026 },
		{1, 65372},
		{-106825736, -2567},
		{0.0, 3.14159265358979},
		{(float)27.1828354, (float)-27.1828354},
		{-65.336, (double)-30006587.2654},
		{(float)-23.3356, -30006587.2654}, };

		for (auto ele : test_number_2) {
			TestCompareNumber<Real>(ele.first, ele.second);
		}
	}
}

TEST_CASE("Test Parse and Tostring for Real.", "[ParseAndToString][Real]") {
	SECTION("Parse Right") {
		std::string test_number_1[] = { 
			"3.14159265358979323846264338327950288e101",
		"5",
		"0e14",
		"-0e0",
		"1e128", 
		"1.32468208213" };

		for (auto ele : test_number_1) {
			Real a, b;
			REQUIRE(b.Parse(ele.c_str()) == Number_Parse_OK);

			REQUIRE(a.Parse(b.ToString10().c_str()) == Number_Parse_OK);
			REQUIRE(a == b);
			REQUIRE(a.ToString10() == b.ToString10());
		}
	}

	SECTION("Parse wrong.") {
		std::string test_number_2[] = { "", "-", "0b1102",
			"0o123467758", "123743a", "0x23c4dfaeg", "0x-3df", "6.658765e"};

		for (auto ele : test_number_2) {
			Real a;
			REQUIRE(a.Parse(ele.c_str()) == Number_Parse_Failed);
			REQUIRE(a == 0);
		}
	}

	SECTION("Random number.") {
		for (int ii = 0;ii < real_test_num;++ii) {
			Real n1 = RandomRealNumber();
			Real n2;
			n2.SetPrecision(n1.GetPrecision());
			n2.Parse(n1.ToString10().c_str());
			REQUIRE(n1 == n2);
		}
	}
}
