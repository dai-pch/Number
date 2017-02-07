#pragma once
#ifndef NUMBER_UINTEGER_H__
#define NUMBER_UINTEGER_H__

#include <iostream>
#include <vector>
#include <string>
#include <type_traits>
#include "Number.h"

namespace Number {
	class UInteger {
		friend class Integer;
		friend class Real;
	public:
		UInteger() = default;
		UInteger(const UInteger&);
		//类型转换
		template<typename UIntType>
		explicit UInteger(const UIntType& Source, typename ::std::enable_if_t<
			::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value>* = nullptr) {
			*this = Source;
		}

		~UInteger() = default;

		// 赋值操作
		//无符号整数
		template<typename UIntType>
		typename ::std::enable_if_t<
			::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, UInteger>&
			operator = (const UIntType& Source)
		{
			this->SetNumber(Source);
			return *this;
		}
		//比较运算
		int Compare(const UInteger&) const; //小于输出负数，大于输出正数，等于输出零

											//逻辑操作符
		UInteger operator<<(int) const;
		UInteger operator>>(int) const;


		// 算数操作
		// Add
		UInteger Add(const UInteger& Number2) const;
		// Sub
		UInteger Sub(const UInteger& Number2) const;
		// Multiply
		UInteger Multiply(const UInteger& Number2) const;
		// Devide
		friend void Devide(const UInteger &Number1, const UInteger &Number2,
			UInteger &Mod, UInteger& quotient);

		//转换为10进制字串
		::std::string ToString10() const;
		::std::string ToString16() const;

		int Parse(::std::string);

		size_t get_digit_number() const;
		//save_type get_highest_digit() const;

		//输入输出
		friend ::std::ostream& operator<<(::std::ostream&, const UInteger&);
		friend ::std::istream& operator>>(::std::istream&, UInteger&);

		//其他算数运算
		UInteger Power(const save_type exp) const;
		UInteger Power(const UInteger &exp) const;

	private:
		::std::vector<save_type> _number{ 0 };

	private:
		UInteger(const ::std::vector<save_type>&);
		UInteger(::std::vector<save_type>&&);

		template<typename T>
		void SetNumber(T num)
		{
			_number.clear();
			do {
				save_type lowbyte, highbyte;
				detail::CalcTypeToSaveType(num, highbyte, lowbyte);
				_number.push_back(lowbyte);
				num = static_cast<T>(highbyte);
			} while (num != 0);
		}

		void FromString10(const ::std::string & c);
		void FromString2(const ::std::string & c);
		void FromString8(const ::std::string & c);
		void FromString16(const ::std::string & c);

	}; // class

	   // compare operator
	   // ==运算符
	inline bool operator == (const UInteger& Number1, const UInteger& Number2) {
		return ((Number1.Compare(Number2)) == 0);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, bool> // bool
		operator == (const UIntType& Number1, const UInteger& Number2) {
		return (Number2 == UInteger(Number1));
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, bool> // bool
		operator == (const UInteger& Number1, const UIntType& Number2) {
		return (Number1 == UInteger(Number2));
	}
	// !=运算符
	inline bool operator != (const UInteger& Number1, const UInteger& Number2) {
		return ((Number1.Compare(Number2)) != 0);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, bool> // bool
		operator != (const UIntType& Number1, const UInteger& Number2) {
		return (Number2 != UInteger(Number1));
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, bool> // bool
		operator != (const UInteger& Number1, const UIntType& Number2) {
		return (Number1 != UInteger(Number2));
	}
	// >运算符
	inline bool operator > (const UInteger &number1, const UInteger &number2) {
		return ((number1.Compare(number2)) > 0);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, bool> // bool
		operator > (const UInteger& Number1, const UIntType& Number2) {
		return (Number1 > UInteger(Number2));
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, bool> // bool
		operator > (const UIntType& Number1, const UInteger& Number2) {
		return (UInteger(Number1) > Number2);
	}
	// <运算符	
	inline bool operator < (const UInteger& Number1, const UInteger& Number2) {
		return ((Number1.Compare(Number2)) < 0);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, bool> // bool
		operator < (const UIntType& Number1, const UInteger& Number2) {
		return (UInteger(Number1) < Number2);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, bool> // bool
		operator < (const UInteger& Number1, const UIntType& Number2) {
		return (Number1 < UInteger(Number2));
	}
	// >=运算符
	inline bool operator >= (const UInteger& Number1, const UInteger& Number2) {
		return ((Number1.Compare(Number2)) >= 0);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, bool> // bool
		operator >= (const UIntType& Number1, const UInteger& Number2) {
		return (UInteger(Number1) >= Number2);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, bool> // bool
		operator >= (const UInteger& Number1, const UIntType& Number2) {
		return (Number1 >= UInteger(Number2));
	}
	// <=运算符
	inline bool operator <= (const UInteger& Number1, const UInteger& Number2) {
		return ((Number1.Compare(Number2)) <= 0);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, bool> // bool
		operator <= (const UIntType& Number1, const UInteger& Number2) {
		return (UInteger(Number1) <= Number2);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, bool> // bool
		operator <= (const UInteger& Number1, const UIntType& Number2) {
		return (Number1 <= UInteger(Number2));
	}
	// operator +
	inline UInteger operator + (const UInteger& Number1, const UInteger& Number2) {
		return Number1.Add(Number2);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, UInteger> // UInteger
		operator + (const UIntType& Number1, const UInteger& Number2) {
		return (Number2 + UInteger(Number1));
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, UInteger> // UInteger
		operator + (const UInteger& Number1, const UIntType& Number2) {
		return (Number1 + UInteger(Number2));
	}
	// operator -
	inline UInteger operator - (const UInteger& Number1, const UInteger& Number2) {
		return Number1.Sub(Number2);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, UInteger> // UInteger
		operator - (const UIntType& Number1, const UInteger& Number2) {
		return (UInteger(Number1) - Number2);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, UInteger> // UInteger
		operator - (const UInteger& Number1, const UIntType& Number2) {
		return (Number1 - UInteger(Number2));
	}
	// operator *
	inline UInteger operator * (const UInteger& Number1, const UInteger& Number2) {
		return Number1.Multiply(Number2);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, UInteger> // UInteger
		operator * (const UIntType& Number1, const UInteger& Number2) {
		return (UInteger(Number1) * Number2);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, UInteger> // UInteger
		operator * (const UInteger& Number1, const UIntType& Number2) {
		return (Number1 * UInteger(Number2));
	}
	// operator /
	inline UInteger operator / (const UInteger& Number1, const UInteger& Number2) {
		UInteger q, r;
		Devide(Number1, Number2, r, q);
		return q;
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, UInteger> // UInteger
		operator / (const UIntType& Number1, const UInteger& Number2) {
		return (UInteger(Number1) / Number2);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, UInteger> // UInteger
		operator / (const UInteger& Number1, const UIntType& Number2) {
		return (Number1 / UInteger(Number2));
	}
	// operator %
	inline UInteger operator % (const UInteger& Number1, const UInteger& Number2) {
		UInteger q, r;
		Devide(Number1, Number2, r, q);
		return r;
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, UInteger> // UInteger
		operator % (const UIntType& Number1, const UInteger& Number2) {
		return (UInteger(Number1) % Number2);
	}
	template<typename UIntType>
	inline ::std::enable_if_t<::std::is_integral<UIntType>::value && ::std::is_unsigned<UIntType>::value, UInteger> // UInteger
		operator % (const UInteger& Number1, const UIntType& Number2) {
		return (Number1 % UInteger(Number2));
	}
}

//#include "UInteger_implement.hpp"

#endif
