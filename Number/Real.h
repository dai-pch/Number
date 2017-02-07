#pragma once
#ifndef _NUMBER_REAL_H__
#define _NUMBER_REAL_H__

#include "Number.h"
#include "UInteger.h"

namespace Number {
	class Real {
	public:
		//构造函数
		Real(size_t precision = default_precision) :_number((precision - 1) / BIT_NUMBER + 2, 0) {} //空构造
		Real(const Real&) = default; //拷贝构造
		Real(const Real& real, size_t precision) :Real(real) {
			this->SetPrecision(precision);
		}
		Real(::std::vector<save_type> vec, signal_type sig = 1) :_number(vec), _signal(sig){}
		explicit Real(const Integer& inte);
		explicit Real(const UInteger& inte);
		//从整数构造
		template<typename T>
		explicit Real(const T& Number, typename ::std::enable_if<
			::std::is_integral<T>::value>::type* = nullptr) :Real(Integer(Number)){}
		//从浮点数构造
		explicit Real(float Number) {
			detail::convertFloatingToInteger(Number, _signal, _number._number, _exp);
		}
		explicit Real(double Number) {
			detail::convertFloatingToInteger(Number, _signal, _number._number, _exp);
		}

		//赋值
		Real& operator=(const Real&);
		Real& operator=(Real&&);
		Real& operator=(const Integer&);
		Real& operator=(const UInteger&);
		template<typename IntType>
		typename ::std::enable_if_t<::std::is_integral<IntType>::value, Real>& 
			operator=(IntType src) {
			*this = Integer(src);
			return *this;
		}
		Real& operator=(float src);
		Real& operator=(double src);
		

		//比较函数
		int Compare(const Real&) const;

		// Arithmetic operation
		// Minus
		Real operator-() const;
		// Add
		Real Add(const Real& num) const;
		// Sub
		Real Sub(const Real& num) const;
		// Multiply
		Real Multiply(const Real& num) const;
		// Devide
		Real Devide(const Real& num) const;


		//转换为字符串
		::std::string ToString10() const;
		int Parse(const ::std::string&);

		//输入输出
		friend ::std::ostream& operator<<(::std::ostream&, const Integer&);
		friend ::std::istream& operator>>(::std::istream&, Integer&);

		static void SetDefaultPrecision(size_t);
		static size_t GetDefaultPrecision();
		void SetPrecision(size_t);
		size_t GetPrecision();

	private:
		static size_t default_precision;
		char _signal{ 1 };
		UInteger _number =
			UInteger(::std::vector<save_type>((default_precision - 1) / BIT_NUMBER + 2, 0));
		exp_type _exp{ 0 }; // radix is MODULE

		std::vector<save_type>& _numvec = _number._number;

		inline void RealParseF(::std::string::const_iterator it,
			const ::std::string::const_iterator end, ::std::vector<save_type>& f,
			exp_type& e);
		inline void RealParseExp(::std::string::const_iterator it,
			const ::std::string::const_iterator end, exp_type& e);

	public:
		
	}; // class

	// ==运算符
	inline bool operator == (const Real &Number1, const Real &Number2) {
		return (Number1.Compare(Number2) == 0);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator == (const Arithmetic& Number1, const Real& Number2) {
		return (Real(Number1) == Number2);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator == (const Real& Number1, const Arithmetic& Number2) {
		return (Number1 == Real(Number2));
	}
	// !=运算符
	inline bool operator != (const Real &Number1, const Real &Number2) {
		return (Number1.Compare(Number2) != 0);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator != (const Arithmetic& Number1, const Real& Number2) {
		return (Real(Number1) != Number2);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator != (const Real& Number1, const Arithmetic& Number2) {
		return (Number1 != Real(Number2));
	}
	// >运算符
	inline bool operator > (const Real &Number1, const Real &Number2) {
		return (Number1.Compare(Number2) > 0);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator > (const Arithmetic& Number1, const Real& Number2) {
		return (Real(Number1) > Number2);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator > (const Real& Number1, const Arithmetic& Number2) {
		return (Number1 > Real(Number2));
	}
	// <运算符	
	inline bool operator < (const Real &Number1, const Real &Number2) {
		return (Number1.Compare(Number2) < 0);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator < (const Arithmetic& Number1, const Real& Number2) {
		return (Real(Number1) < Number2);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator < (const Real& Number1, const Arithmetic& Number2) {
		return (Number1 < Real(Number2));
	}
	// >=运算符
	inline bool operator >= (const Real& Number1, const Real& Number2) {
		return (Number1.Compare(Number2) >= 0);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator >= (const Arithmetic& Number1, const Real& Number2) {
		return (Real(Number1) >= Number2);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator >= (const Real& Number1, const Arithmetic& Number2) {
		return (Number1 >= Real(Number2));
	}
	// <=运算符
	inline bool operator <= (const Real& Number1, const Real& Number2) {
		return (Number1.Compare(Number2) <= 0);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator <= (const Arithmetic& Number1, const Real& Number2) {
		return (Real(Number1) <= Number2);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator <= (const Real& Number1, const Arithmetic& Number2) {
		return (Number1 <= Real(Number2));
	}

	// Arithmetic operation
	// +
	inline Real operator + (const Real& number1, const Real& number2) {
		return number1.Add(number2);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator + (const Arithmetic& Number1, const Real& Number2)
	{
		return Real(Number1) + Number2;
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator + (const Real& Number1, const Arithmetic& Number2)
	{
		return Number1 + Real(Number2);
	}
	// -
	inline Real operator - (const Real& number1, const Real& number2) {
		return number1.Sub(number2);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator - (const Arithmetic& Number1, const Real& Number2)
	{
		return Real(Number1) - Number2;
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator - (const Real& Number1, const Arithmetic& Number2)
	{
		return Number1 - Real(Number2);
	}
	// *
	inline Real operator * (const Real& number1, const Real& number2) {
		return number1.Multiply(number2);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator * (const Arithmetic& Number1, const Real& Number2)
	{
		return Real(Number1) * Number2;
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator * (const Real& Number1, const Arithmetic& Number2)
	{
		return Number1 * Real(Number2);
	}
	// /
	inline Real operator / (const Real& number1, const Real& number2) {
		return number1.Devide(number2);
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator / (const Arithmetic& Number1, const Real& Number2)
	{
		return Real(Number1) / Number2;
	}
	template<typename Arithmetic>
	inline typename ::std::enable_if_t<
		::std::is_arithmetic<Arithmetic>::value
		|| ::std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator / (const Real& Number1, const Arithmetic& Number2)
	{
		return Number1 / Real(Number2);
	}
}

#endif
