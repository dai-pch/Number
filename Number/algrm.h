#pragma once
#ifndef _NUMBER_ALGRM_H
#define _NUMBER_ALGRM_H

#include "Number.h"

namespace Number {

	template<typename T>
	T Devide(const T& Obj1, const T& Obj2, T &mod, typename std::enable_if_t<
		std::is_integral<T>::value>* = nullptr)
	{
		mod = Obj1 % Obj2;
		return Obj1 / Obj2;
	}

	template<typename T>
	T ExGCD(const T &a, const T &b, T &x, T &y,
		typename std::enable_if_t<
		(std::is_integral<T>::value) || (std::is_same<T, Integer>::value)
		>* = nullptr)
	{
		if (a < b)
			return ExGCD(b, a, y, x);
		else if (b == 0)
		{
			x = 1;
			y = 0;
			return a;
		}
		else
		{
			T r, q;
			Devide(a, b, r, q);
			r = ExGCD(b, r, y, x);
			y = y - q * x;
			return r;
		}
	}

}

#endif