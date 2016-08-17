#pragma once
#ifndef INTEGER_H
#define INTEGER_H

#include <vector>
#include <algorithm>
#include <type_traits>

using namespace std;

//所用类型的定义，根据平台不同进行改动
#define int64 __int64
#define uint64 unsigned __int64
#define int32 __int32
#define uint32 unsigned __int32

//定义存储进制
#define MODULE 0x100000000//4294967296 //2^32

typedef uint32 save_type;
typedef uint64 calc_type_u;
typedef int64 calc_type;

class Integer {
private:
	char _signal = 1;
	vector<save_type> _number{ 0 };

	Integer(vector<save_type> Number, char Signal = 1);

	//大于
	//32位以下整数
	template<typename T>
	inline bool LargerThan(const T& Number2,
		typename std::enable_if<(sizeof T <= sizeof save_type) &&
		(std::is_integral<T>::value)>::type* = nullptr) const
	{
		return (_number.size() == 1) ?
			(_signal*_number[0]) > (calc_type)Number2 : (_signal > 0);
	}
	//32位以上整数
	template<typename T>
	inline bool LargerThan(const T& Number2,
		typename std::enable_if<(sizeof T > sizeof save_type) &&
		(std::is_integral<T>::value)>::type* = nullptr) const
	{
		return *this > Integer(Number2);
	}
	//小于
	//32位以下整数
	template<typename T>
	inline bool SmallerThan(const T& Number2,
		typename std::enable_if<(sizeof T <= sizeof save_type) &&
		(std::is_integral<T>::value)>::type* = nullptr) const
	{
		return (_number.size() == 1) ?
			((_signal*_number[0]) < (calc_type)Number2) : (_signal < 0);
	}
	//32位以上整数
	template<typename T>
	inline bool SmallerThan(const T& Number2,
		typename std::enable_if<(sizeof T > sizeof save_type) &&
		(std::is_integral<T>::value)>::type* = nullptr) const
	{
		return *this < Integer(Number2);
	}
	//等于
	//32位以下整数
	template<typename T>
	inline bool EqualWith(const T& Number2,
		typename std::enable_if<(sizeof T <= sizeof save_type) &&
		(std::is_integral<T>::value)>::type* = nullptr) const
	{
		return (_number.size() == 1) ?
			((_signal*_number[0]) == (calc_type)Number2) : false;
	}
	//32位以上整数
	template<typename T>
	inline bool EqualWith(const T& Number2,
		typename std::enable_if<(sizeof T > sizeof save_type) &&
		(std::is_integral<T>::value)>::type* = nullptr) const
	{
		return *this == Integer(Number2);
	}

public:
	Integer() {}
	Integer(const Integer&);

	template<typename T>
	Integer(const T& Source,
		typename std::enable_if<std::is_integral<T>::value>::type* = nullptr)
	{
		T temp = Source < 0 ? (_signal = -1, -Source) : (_signal = 1, Source);
		_number.reserve(sizeof(T) / sizeof(save_type) + 1);
		while (temp != 0)
		{
			_number.push_back(temp % MODULE);
			temp /= MODULE;
		}
	}

	//Integer(const int);
	//Integer(const unsigned int);
	~Integer() {}


	int setSignal(const int& signal);
	//int negative();


	//比较运算
	//>运算符
	bool operator>(const Integer&) const;
	template<typename T>
	friend inline bool operator>(const Integer& Obj1, const T& Obj2)
	{
		return Obj1.LargerThan(Obj2);
	}
	template<typename T>
	friend inline bool operator>(const T& Obj1, const Integer& Obj2)
	{
		return Obj2.SmallerThan(Obj1);
	}
	//<运算符
	bool operator<(const Integer&) const;
	//其他类型
	template<typename T>
	friend inline bool operator<(const Integer& Obj1, const T& Obj2)// const
	{
		return Obj1.SmallerThan(Obj2);
	}
	template<typename T>
	friend inline bool operator<(const T& Obj1, const Integer& Obj2)// const
	{
		return Obj2.LargerThan(Obj1);
	}
	//==运算符
	bool operator==(const Integer&) const;
	//其他类型
	template<typename T>
	friend inline bool operator==(const Integer& Obj1, const T& Obj2)
	{
		return Obj1.EqualWith(Obj2);
	}
	template<typename T>
	friend inline bool operator==(const T& Obj1, const Integer& Obj2)
	{
		return Obj2.EqualWith(Obj1);
	}
	//>=运算符
	friend inline bool operator>=(const Integer& Number1, const Integer& Number2)
	{
		return !(Number1 < Number2);
	}
	template<typename T>
	friend inline bool operator>=(const Integer& Number1, const T& Number2)
	{
		return !(Number1 < Number2);
	}
	template<typename T>
	friend inline bool operator>=(const T& Number1, const Integer& Number2)
	{
		return !(Number1 < Number2);
	}
	//<=运算符
	friend inline bool operator<=(const Integer& Number1, const Integer& Number2)
	{
		return !(Number1 > Number2);
	}
	template<typename T>
	friend inline bool operator<=(const Integer& Number1, const T& Number2)
	{
		return !(Number1 > Number2);
	}
	template<typename T>
	friend inline bool operator<=(const T& Number1, const Integer& Number2)
	{
		return !(Number1 > Number2);
	}

	Integer operator-() const;
	Integer operator+(const Integer&) const;
	Integer operator-(const Integer&) const;





	

};

#endif