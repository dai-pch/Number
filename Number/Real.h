#pragma once
#ifndef _NUMBER_REAL_H__
#define _NUMBER_REAL_H__

#include "Number.h"
#include "Integer.h"

namespace Number {
	class Real {
	public:
		//���캯��
		Real(){} //�չ���
		Real(Real&); //��������
		Real(Integer&); 
		template<typename T>
		Real(T& Number, typename std::enable_if<
		std::is_floating_point<T>::value>::type* = nullptr) { //�Ӹ���������

		}

		//�ȽϺ���
		int Compare(const Real&) const;
		//>�����
		bool operator>(const Real &Number2) const {
			return ((this->Compare(Number2)) > 0);
		}
		template<typename T>
		friend bool operator>(const T& Number1, const Real& Number2)
		{
			return Number2 < Number1;
		}
		//<�����	
		bool operator<(const Real& Number2) const {
			return ((this->Compare(Number2)) < 0);
		}
		template<typename T>
		friend bool operator<(const T& Number1, const Real& Number2)
		{
			return Number2 > Number1;
		}
		//==�����
		bool operator==(const Real& Number2) const {
			return ((this->Compare(Number2)) == 0);
		}
		template<typename T>
		friend bool operator==(const T& Number1, const Real& Number2)
		{
			return Number2 == Number1;
		}
		//!=�����
		bool operator!=(const Real& Number2) const
		{
			return ((this->Compare(Number2)) != 0);
		}
		template<typename T>
		friend bool operator!=(const T& Number1, const Real& Number2)
		{
			return (Number2 != Number1);
		}
		//>=�����
		bool operator>=(const Real& Number2) const
		{
			return ((this->Compare(Number2)) >= 0);
		}
		template<typename T>
		friend bool operator>=(const T& Number1, const Real& Number2)
		{
			return (Number2 <= Number1);
		}
		//<=�����
		bool operator<=(const Real& Number2) const
		{
			return ((this->Compare(Number2)) <= 0);
		}
		template<typename T>
		friend bool operator<=(const T& Number1, const Real& Number2)
		{
			return (Number2 >= Number1);
		}

		//����������
		Real operator-() const;

		Real operator+(const Integer&) const;
		template<typename T>
		friend Real operator+(const T& Number1, const Real& Number2)
		{
			return Number2 + Number1;
		}

		Real operator-(const Real&) const;
		template<typename T>
		friend Real operator-(const T& Number1, const Real& Number2)
		{
			return (-Number2) + Number1;
		}

		Real operator*(const Real&) const;
		template<typename T>
		friend Real operator*(const T& Number1, const Real& Number2)
		{
			return Number2 * Number1;
		}

		Real operator/(const Real& Number2) const;
		template<typename T>
		friend Real operator/(const T& Number1, const Real& Number2)
		{
			Real num1 = Number1;
			return num1 / Number2;
		}

		//ת��Ϊ�ַ���
		::std::string ToString10() const;
		::std::string ToString16() const;
		int Parse(std::string);

		//�������
		friend ::std::ostream& operator<<(::std::ostream&, const Integer&);
		friend ::std::istream& operator>>(::std::istream&, Integer&);

	private:
		Integer _number{0};
		int _exp{0};
		save_type _tolerance{0};
		int _tolexp{0};
	};
}

#endif
