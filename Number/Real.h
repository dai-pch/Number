#pragma once
#ifndef _NUMBER_REAL_H__
#define _NUMBER_REAL_H__

#include "Number.h"
#include "Integer.h"

namespace Number {
	class Real {
	public:
		//���캯��
		Real() = default; //�չ���
		Real(const Real& num) = default; //��������
		Real(const Integer&); 
		//����������
		template<typename T>
		Real(const T& Number, typename std::enable_if<
			std::is_integral<T>::value>::type* = nullptr) :Real(Integer(Number)){}
		//�Ӹ���������
		template<typename T>
		Real(const T& Number, typename std::enable_if<
		std::is_floating_point<T>::value>::type* = nullptr) { 

		}

		//�ȽϺ���
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


		//ת��Ϊ�ַ���
		::std::string ToString10() const;
		::std::string ToString16() const;
		int Parse(std::string);

		//�������
		friend ::std::ostream& operator<<(::std::ostream&, const Integer&);
		friend ::std::istream& operator>>(::std::istream&, Integer&);

	private:
		Integer _number;
		int _exp = 0;
		save_type _tolerance = 0;
		int _tolexp = 0;
	}; // class

	// ==�����
	inline bool operator == (const Real &Number1, const Real &Number2) {
		return (Number1.Compare(Number2) == 0);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator == (const Arithmetic& Number1, const Real& Number2) {
		return (Real(Number1) == Number2);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator == (const Real& Number1, const Arithmetic& Number2) {
		return (Number1 == Real(Number2));
	}
	// !=�����
	inline bool operator != (const Real &Number1, const Real &Number2) {
		return (Number1.Compare(Number2) != 0);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator != (const Arithmetic& Number1, const Real& Number2) {
		return (Real(Number1) != Number2);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator != (const Real& Number1, const Arithmetic& Number2) {
		return (Number1 != Real(Number2));
	}
	// >�����
	inline bool operator > (const Real &Number1, const Real &Number2) {
		return (Number1.Compare(Number2) > 0);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator > (const Arithmetic& Number1, const Real& Number2) {
		return (Real(Number1) > Number2);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator > (const Real& Number1, const Arithmetic& Number2) {
		return (Number1 > Real(Number2));
	}
	// <�����	
	inline bool operator < (const Real &Number1, const Real &Number2) {
		return (Number1.Compare(Number2) < 0);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator < (const Arithmetic& Number1, const Real& Number2) {
		return (Real(Number1) < Number2);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator < (const Real& Number1, const Arithmetic& Number2) {
		return (Number1 < Real(Number2));
	}
	// >=�����
	inline bool operator >= (const Real& Number1, const Real& Number2) {
		return (Number1.Compare(Number2) >= 0);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator >= (const Arithmetic& Number1, const Real& Number2) {
		return (Real(Number1) >= Number2);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator >= (const Real& Number1, const Arithmetic& Number2) {
		return (Number1 >= Real(Number2));
	}
	// <=�����
	inline bool operator <= (const Real& Number1, const Real& Number2) {
		return (Number1.Compare(Number2) <= 0);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator <= (const Arithmetic& Number1, const Real& Number2) {
		return (Real(Number1) <= Number2);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, bool> // bool
		operator <= (const Real& Number1, const Arithmetic& Number2) {
		return (Number1 <= Real(Number2));
	}

	// Arithmetic operation
	// +
	inline Real operator + (const Real& number1, const Real& number2) {
		return number1.Add(number2);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator + (const Arithmetic& Number1, const Real& Number2)
	{
		return Real(Number1) + Number2;
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator + (const Real& Number1, const Arithmetic& Number2)
	{
		return Number1 + Real(Number2);
	}
	// -
	inline Real operator - (const Real& number1, const Real& number2) {
		return number1.Sub(number2);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator - (const Arithmetic& Number1, const Real& Number2)
	{
		return Real(Number1) - Number2;
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator - (const Real& Number1, const Arithmetic& Number2)
	{
		return Number1 - Real(Number2);
	}
	// *
	inline Real operator * (const Real& number1, const Real& number2) {
		return number1.Multiply(number2);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator * (const Arithmetic& Number1, const Real& Number2)
	{
		return Real(Number1) * Number2;
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator * (const Real& Number1, const Arithmetic& Number2)
	{
		return Number1 * Real(Number2);
	}
	// /
	inline Real operator / (const Real& number1, const Real& number2) {
		return number1.Devide(number2);
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator / (const Arithmetic& Number1, const Real& Number2)
	{
		return Real(Number1) / Number2;
	}
	template<typename Arithmetic>
	inline typename std::enable_if_t<
		std::is_arithmetic<Arithmetic>::value
		|| std::is_same<Arithmetic, Integer>::value, Real>          //Real
		operator / (const Real& Number1, const Arithmetic& Number2)
	{
		return Number1 / Real(Number2);
	}
}

#endif
