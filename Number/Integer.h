#pragma once
#ifndef INTEGER_H
#define INTEGER_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <type_traits>

//debug
#define DEBUG_MODE false

using namespace std;

//所用类型的定义，根据平台不同进行改动
#define int64 __int64
#define uint64 unsigned __int64
#define int32 __int32
#define uint32 unsigned __int32

typedef uint32 save_type;
typedef uint64 calc_type_u;
typedef int64 calc_type;

//定义存储进制
#define MODULE 0x100000000//4294967296 //2^32
#define BYTE_NUMBER sizeof(save_type)
#define BIT_NUMBER sizeof(save_type)*8

class Integer {
private:
	char _signal = 1;
	vector<save_type> _number{ 0 };

	Integer(vector<save_type> Number, char Signal = 1);
	
	template<typename T>
	void SetNumber(const T &num)
	{
		T temp = num;
		_number.reserve((sizeof(T) + 3) / sizeof(save_type));
		do {
			_number.push_back(static_cast<save_type>(temp % MODULE));
			temp = static_cast<T>(temp / MODULE);
		} while (temp != 0);
	}


	void FromString10(string::const_iterator &it,
		const string::const_iterator &end, const char &signal);
	void FromString2(string::const_iterator &it,
		const string::const_iterator &end, const char &signal);
	void FromString8(string::const_iterator &it,
		const string::const_iterator &end, const char &signal);
	void FromString16(string::const_iterator &it,
		const string::const_iterator &end, const char &signal);


public:
	Integer() {}
	Integer(const Integer&);

	//类型转换
	//无符号整数
	template<typename T>
	Integer(const T& Source, typename std::enable_if<
		std::is_integral<T>::value && std::is_unsigned<T>::value
		>::type* = nullptr)
		: _number(), _signal(1)
	{
		this->SetNumber(Source);
	}
	//有符号整数
	template<typename T>
	Integer(const T& Source, typename std::enable_if<
		std::is_integral<T>::value && std::is_signed<T>::value
		>::type* = nullptr) : _number(0), _signal(1)
	{
		 std::make_unsigned_t<T> temp = (Source < 0 ? (_signal = -1, -Source) : Source);
		 this->SetNumber(temp);
	}
	//浮点数
	/*template<typename T>
	Integer(const T& Source, typename std::enable_if<
		std::is_floating_point<T>::value
	>::type* = nullptr) : _number(0), _signal(1)
	{
		T temp = (Source < 0 ? (_signal = -1, -Source) : Source);
		
	}*/


	~Integer() {}


	int setSignal(const int& signal);
	//int negative();


	//比较运算
	//>运算符
	bool operator>(const Integer&) const;
	template<typename T>
	friend bool operator>(const T& Obj1, const Integer& Obj2)
	{
		return Obj2 < Obj1;
	}
	//<运算符
	bool operator<(const Integer&) const;
	template<typename T>
	friend bool operator<(const T& Obj1, const Integer& Obj2)
	{
		return Obj2 > Obj1;
	}
	//==运算符
	bool operator==(const Integer&) const;
	template<typename T>
	friend bool operator==(const T& Obj1, const Integer& Obj2)
	{
		return Obj2 == Obj1;
	}
	//!=运算符
	bool operator!=(const Integer& Obj2) const
	{
		return !(*this == Obj2);
	}
	template<typename T>
	friend bool operator!=(const T& Obj1, const Integer& Obj2)
	{
		return !(Obj2 == Obj1);
	}
	//>=运算符
	bool operator>=(const Integer& Number2) const
	{
		return !(*this < Number2);
	}
	template<typename T>
	friend bool operator>=(const T& Number1, const Integer& Number2)
	{
		return !(Number2 > Number1);
	}
	//<=运算符
	bool operator<=(const Integer& Number2) const
	{
		return !(*this > Number2);
	}
	template<typename T>
	friend bool operator<=(const T& Number1, const Integer& Number2)
	{
		return !(Number2 < Number1);
	}


	//逻辑操作符
	Integer operator<<(const int&) const;

	Integer operator>>(const int&) const;

	
	//算数操作符
	Integer operator-() const;

	Integer operator+(const Integer&) const;
	template<typename T>
	friend Integer operator+(const T& Number1, const Integer& Number2)
	{
		return Number2 + Number1;
	}

	Integer operator-(const Integer&) const;
	template<typename T>
	friend Integer operator-(const T& Number1, const Integer& Number2)
	{
		return (-Number2) + Number1;
	}

	Integer operator*(const Integer&) const;
	template<typename T>
	friend Integer operator*(const T& Number1, const Integer& Number2)
	{
		return Number2 * Number1;
	}

	friend Integer _Devide(const Integer& Obj1, const Integer& Obj2, Integer& mod);

	Integer operator/(const Integer& Num2) const
	{
		Integer mod;
		return _Devide(*this, Num2, mod);
	}
	template<typename T>
	Integer operator/(const T& Num2) const
	{
		Integer mod;
		return _Devide(*this, Num2, mod);
	}
	template<typename T>
	friend Integer operator/(const T& Num1, const Integer& Num2)
	{
		Integer mod;
		return _Devide(Integer(Num1), Num2, mod);
	}

	Integer operator%(const Integer& Num2) const
	{
		Integer mod;
		_Devide(*this, Num2, mod);
		return mod;
	}
	template<typename T>
	Integer operator%(const T& Num2) const
	{
		Integer mod;
		_Devide(*this, Num2, mod);
		return mod;
	}
	template<typename T>
	friend Integer operator%(const T& Num1, const Integer& Num2)
	{
		Integer mod;
		_Devide(Integer(Num1), Num2, mod);
		return mod;
	}


	//转换为10进制字串
	string ToString10() const;
	string ToString16() const;

	void Parse(const string&);

	//输入输出
	friend ostream& operator<<(ostream&, const Integer&);
	friend istream& operator>>(istream&, Integer&);

	//其他辅助操作
	unsigned char backbit() const;

	//其他算数运算
	Integer Abs() const;

	Integer Power(const Integer &exp) const;

};


#endif