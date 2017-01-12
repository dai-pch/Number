#pragma once
#ifndef _NUMBER_INTEGER_H__
#define _NUMBER_INTEGER_H__

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include "Number.h"

namespace Number {
	class Integer {
		friend class Real;
	public:
		Integer() = default;
		Integer(const Integer&);

		//类型转换
		template<typename T>
		explicit Integer(const T& Source) {
			*this = Source;
		}
		/*//无符号整数
		template<typename UIntType>
		explicit Integer(const UIntType& Source, typename std::enable_if_t<
			std::is_integral<UIntType>::value && std::is_unsigned<UIntType>::value
		>* = nullptr)
		{
			*this = Source;
		}
		//有符号整数
		template<typename IntType>
		explicit Integer(const IntType& Source, typename std::enable_if_t<
			std::is_integral<IntType>::value && std::is_signed<IntType>::value
		>* = nullptr)
		{
			*this = Source;
		}*/
		//浮点数
		/*template<typename T>
		Integer(const T& Source, typename std::enable_if<
		std::is_floating_point<T>::value
		>::type* = nullptr) : _number(0), _signal(1)
		{
		T temp = (Source < 0 ? (_signal = -1, -Source) : Source);

		}*/

		~Integer() {}

		// 赋值操作
		//无符号整数
		template<typename UIntType>
		typename std::enable_if_t<
			std::is_integral<UIntType>::value && std::is_unsigned<UIntType>::value, Integer>&
			operator = (const UIntType& Source)
		{
			this->SetNumber(Source);
			return *this;
		}
		//有符号整数
		template<typename IntType>
		typename std::enable_if_t<
			std::is_integral<IntType>::value && std::is_signed<IntType>::value, Integer>& 
			operator = (const IntType& Source)
		{
			std::make_unsigned_t<IntType> temp = (Source < 0 ? (_signal = -1, -Source) : Source);
			this->SetNumber(temp);
			return *this;
		}
		//比较运算
		int Compare(const Integer&) const; //小于输出负数，大于输出正数，等于输出零

		//逻辑操作符
		Integer operator<<(int) const;
		Integer operator>>(int) const;


		// 算数操作
		// Minus
		Integer operator-() const;
		// Add
		Integer Add(const Integer& Number2) const;
		// Sub
		Integer Sub(const Integer& Number2) const;
		// Multiply
		Integer Multiply(const Integer& Number2) const;
		// Devide
		friend void Devide(const Integer &Number1, const Integer &Number2,
			Integer &Mod, Integer& quotient);

		//转换为10进制字串
		std::string ToString10() const;
		std::string ToString16() const;

		int Parse(std::string);

		size_t get_digit_number() const;
		//save_type get_highest_digit() const;

		//输入输出
		friend ::std::ostream& operator<<(::std::ostream&, const Integer&);
		friend ::std::istream& operator>>(::std::istream&, Integer&);

		//其他算数运算
		Integer Abs() const;
		Integer Power(const Integer &exp) const;

		friend class Real;

	private:
		char _signal = 1;
		::std::vector<save_type> _number{ 0 };

		Integer(::std::vector<save_type>& Number, char Signal = 1);

		template<typename T>
		void SetNumber(const T &num)
		{
			T temp = num;
			_number.clear();
			do {
				save_type lowbyte, highbyte;
				Number::detail::CalcTypeToSaveType(temp, highbyte, lowbyte);
				_number.push_back(lowbyte);
				temp = static_cast<T>(highbyte);
			} while (temp != 0);
		}

		int setSignal(const int& signal);
		//int negative();

		//其他辅助操作
		int do_integer_compare(const Integer&, size_t&) const;

		unsigned char backbit() const;

		void FromString10(const std::string & c);
		void FromString2(const std::string & c);
		void FromString8(const std::string & c);
		void FromString16(const std::string & c);

	}; // class

	// compare operator
	// ==运算符
	inline bool operator == (const Integer& Number1, const Integer& Number2) {
		return ((Number1.Compare(Number2)) == 0);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator == (const IntType& Number1, const Integer& Number2) {
		return (Number2 == Integer(Number1));
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator == (const Integer& Number1, const IntType& Number2) {
		return (Number1 == Integer(Number2));
	}
	// !=运算符
	inline bool operator != (const Integer& Number1, const Integer& Number2) {
		return ((Number1.Compare(Number2)) != 0);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator != (const IntType& Number1, const Integer& Number2) {
		return (Number2 != Integer(Number1));
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator != (const Integer& Number1, const IntType& Number2) {
		return (Number1 != Integer(Number2));
	}
	// >运算符
	inline bool operator > (const Integer &number1, const Integer &number2) {
		return ((number1.Compare(number2)) > 0);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator > (const Integer& Number1, const IntType& Number2)	{
		return (Number1 > Integer(Number2));
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator > (const IntType& Number1, const Integer& Number2)	{
		return (Integer(Number1) > Number2);
	}
	// <运算符	
	inline bool operator < (const Integer& Number1, const Integer& Number2) {
		return ((Number1.Compare(Number2)) < 0);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator < (const IntType& Number1, const Integer& Number2)	{
		return (Integer(Number1) < Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator < (const Integer& Number1, const IntType& Number2)	{
		return (Number1 < Integer(Number2));
	}
	// >=运算符
	inline bool operator >= (const Integer& Number1, const Integer& Number2) {
		return ((Number1.Compare(Number2)) >= 0);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator >= (const IntType& Number1, const Integer& Number2) {
		return (Integer(Number1) >= Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator >= (const Integer& Number1, const IntType& Number2) {
		return (Number1 >= Integer(Number2));
	}
	// <=运算符
	inline bool operator <= (const Integer& Number1, const Integer& Number2) {
		return ((Number1.Compare(Number2)) <= 0);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator <= (const IntType& Number1, const Integer& Number2) {
		return (Integer(Number1) <= Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, bool> // bool
		operator <= (const Integer& Number1, const IntType& Number2) {
		return (Number1 <= Integer(Number2));
	}
	// operator +
	inline Integer operator + (const Integer& Number1, const Integer& Number2) {
		return Number1.Add(Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator + (const IntType& Number1, const Integer& Number2) {
		return (Number2 + Integer(Number1));
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator + (const Integer& Number1, const IntType& Number2) {
		return (Number1 + Integer(Number2));
	}
	// operator -
	inline Integer operator - (const Integer& Number1, const Integer& Number2) {
		return Number1.Sub(Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator - (const IntType& Number1, const Integer& Number2) {
		return (Integer(Number1) - Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator - (const Integer& Number1, const IntType& Number2) {
		return (Number1 - Integer(Number2));
	}
	// operator *
	inline Integer operator * (const Integer& Number1, const Integer& Number2) {
		return Number1.Multiply(Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator * (const IntType& Number1, const Integer& Number2) {
		return (Integer(Number1) * Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator * (const Integer& Number1, const IntType& Number2) {
		return (Number1 * Integer(Number2));
	}
	// operator /
	inline Integer operator / (const Integer& Number1, const Integer& Number2) {
		Integer q, r;
		Devide(Number1, Number2, r, q);
		return q;
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator / (const IntType& Number1, const Integer& Number2) {
		return (Integer(Number1) / Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator / (const Integer& Number1, const IntType& Number2) {
		return (Number1 / Integer(Number2));
	}
	// operator %
	inline Integer operator % (const Integer& Number1, const Integer& Number2) {
		Integer q, r;
		Devide(Number1, Number2, r, q);
		return r;
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator % (const IntType& Number1, const Integer& Number2) {
		return (Integer(Number1) % Number2);
	}
	template<typename IntType>
	inline std::enable_if_t<std::is_integral<IntType>::value, Integer> // Integer
		operator % (const Integer& Number1, const IntType& Number2) {
		return (Number1 % Integer(Number2));
	}
}

//#include "Integer_implement.hpp"

#endif