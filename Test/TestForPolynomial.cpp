#include "catch.hpp"

#include "../Number/Function.h"
using namespace Number;

TEST_CASE("Test calculation of Polynomial", "[Calculation][Polynomial]") {
	double test_number[] = { 0, 1, 2, 10, 256 };

	Polynomial<int, double> poly;

	for (auto val : test_number) {
		double res = poly(val);
		REQUIRE(res == 0);
	}

	poly.SetCoeffcients(std::vector<int>{1, 2, 1});

	for (auto val : test_number) {
		double res = poly(val);
		double v = val + 1;
		v = v*v;
		REQUIRE(res == v);
	}
}

TEST_CASE("Test reduction of Polynomial", "[Reduction][Polynomial]") {
	double test_number[] = { 0, 1, 2, 10, 256 };

	Polynomial<int, double> poly1;
	Polynomial<int, double> poly2;
	Polynomial<int, double> poly;

	poly1.SetCoeffcients(std::vector<int>{0, 0, 1});
	poly2.SetCoeffcients(std::vector<int>{1, 1});
	poly = poly1(poly2);

	for (auto val : test_number) {
		double res = poly(val);
		double v = val + 1;
		v = v*v;
		REQUIRE(res == v);
	}
}

TEST_CASE("Test AddSub of Polynomial", "[AddSub][Polynomial]") {
	double test_number[] = { 0, 1, 2, 10, 256 };

	Polynomial<int, double> poly1;
	Polynomial<int, double> poly2;
	Polynomial<int, double> polya;
	Polynomial<int, double> polys;

	poly1.SetCoeffcients(std::vector<int>{0, 0, 1});
	poly2.SetCoeffcients(std::vector<int>{1, 1});
	polya = poly1 + poly2;
	polys = poly1 - poly2;

	for (auto val : test_number) {
		double resa = polya(val);
		double ress = polys(val);
		double v1 = val * val;
		double v2 = val + 1;
		REQUIRE(resa == v1 + v2);
		REQUIRE(ress == v1 - v2);
	}
}

TEST_CASE("Test FindRoot of Polynomial", "[FindRoot][Polynomial]") {

	Polynomial<int, double> poly;

	poly.SetCoeffcients(std::vector<int>{1, -2, 1});
	auto res = poly.Root();
	REQUIRE(res.size() == 1);

	poly.SetCoeffcients(std::vector<int>{0, 3, -3, 1});
	res = poly.Root();
	REQUIRE(res.size() == 1);

	poly.SetCoeffcients(std::vector<int>{-1, 0, 0, 1});
	res = poly.Root();
	REQUIRE(res.size() == 3);
}
