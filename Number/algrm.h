#pragma once
#ifndef ALGRM_H
#define ALGRM_H

template<typename T>
T _Devide(const T& Obj1, const T& Obj2, T &mod, typename std::enable_if_t<
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
		T temp, r;
		r = _Devide(a, b, temp);
		temp = ExGCD(b, temp, y, x);
		y = y - r * x;
		return temp;
	}
}



#endif