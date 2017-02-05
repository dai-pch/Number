#pragma once
#ifndef _NUMBER_REAL_H__
#define _NUMBER_REAL_H__

#include "Number.h"

namespace Number {
	class Real {
	public:
		//���캯��
		Real() = default; //�չ���
		Real(const Real&) = default; //��������
		Real(::std::vector<save_type> vec, signal_type sig = 1) :_number(vec), _signal(sig){}
		explicit Real(const Integer& inte);
		//����������
		template<typename T>
		explicit Real(const T& Number, typename ::std::enable_if<
			::std::is_integral<T>::value>::type* = nullptr) :Real(Integer(Number)){}
		//�Ӹ���������
		explicit Real(const float& Number) {
			detail::convertFloatingToInteger(Number, _signal, _number, _exp);
		}
		explicit Real(const double& Number) {
			detail::convertFloatingToInteger(Number, _signal, _number, _exp);
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
		int Parse(const ::std::string&);

		//�������
		friend ::std::ostream& operator<<(::std::ostream&, const Integer&);
		friend ::std::istream& operator>>(::std::istream&, Integer&);

	private:
		char _signal{ 1 };
		::std::vector<save_type> _number{ 0 };
		exp_type _exp{ 0 };
	}; // class

	// ==�����
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
	// !=�����
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
	// >�����
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
	// <�����	
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
	// >=�����
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
	// <=�����
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
