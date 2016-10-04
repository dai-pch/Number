#pragma once
#ifndef _NUMBER_INTEGER_H__
#define _NUMBER_INTEGER_H__

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <type_traits>
#include "Number.h"

namespace Number {

	class Integer {
	private:
		char _signal = 1;
		::std::vector<save_type> _number{ 0 };

		Integer(::std::vector<save_type> Number, char Signal = 1);

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


		void FromString10(::std::string::const_iterator &it,
			const ::std::string::const_iterator &end, const char &signal);
		void FromString2(::std::string::const_iterator &it,
			const ::std::string::const_iterator &end, const char &signal);
		void FromString8(::std::string::const_iterator &it,
			const ::std::string::const_iterator &end, const char &signal);
		void FromString16(::std::string::const_iterator &it,
			const ::std::string::const_iterator &end, const char &signal);


	public:
		Integer() {}
		Integer(const Integer&);

		//����ת��
		//�޷�������
		template<typename T>
		Integer(const T& Source, typename std::enable_if<
			std::is_integral<T>::value && std::is_unsigned<T>::value
		>::type* = nullptr)
			: _number(), _signal(1)
		{
			this->SetNumber(Source);
		}
		//�з�������
		template<typename T>
		Integer(const T& Source, typename std::enable_if<
			std::is_integral<T>::value && std::is_signed<T>::value
		>::type* = nullptr) : _number(0), _signal(1)
		{
			std::make_unsigned_t<T> temp = (Source < 0 ? (_signal = -1, -Source) : Source);
			this->SetNumber(temp);
		}
		//������
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


		//�Ƚ�����
		//>�����
		bool operator>(const Integer&) const;
		template<typename T>
		friend bool operator>(const T& Obj1, const Integer& Obj2)
		{
			return Obj2 < Obj1;
		}
		//<�����
		bool operator<(const Integer&) const;
		template<typename T>
		friend bool operator<(const T& Obj1, const Integer& Obj2)
		{
			return Obj2 > Obj1;
		}
		//==�����
		bool operator==(const Integer&) const;
		template<typename T>
		friend bool operator==(const T& Obj1, const Integer& Obj2)
		{
			return Obj2 == Obj1;
		}
		//!=�����
		bool operator!=(const Integer& Obj2) const
		{
			return !(*this == Obj2);
		}
		template<typename T>
		friend bool operator!=(const T& Obj1, const Integer& Obj2)
		{
			return !(Obj2 == Obj1);
		}
		//>=�����
		bool operator>=(const Integer& Number2) const
		{
			return !(*this < Number2);
		}
		template<typename T>
		friend bool operator>=(const T& Number1, const Integer& Number2)
		{
			return !(Number2 > Number1);
		}
		//<=�����
		bool operator<=(const Integer& Number2) const
		{
			return !(*this > Number2);
		}
		template<typename T>
		friend bool operator<=(const T& Number1, const Integer& Number2)
		{
			return !(Number2 < Number1);
		}


		//�߼�������
		Integer operator<<(const int&) const;

		Integer operator >> (const int&) const;


		//����������
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


		//ת��Ϊ10�����ִ�
		::std::string ToString10() const;
		::std::string ToString16() const;

		void Parse(const ::std::string&);

		//�������
		friend ::std::ostream& operator<<(::std::ostream&, const Integer&);
		friend ::std::istream& operator>>(::std::istream&, Integer&);

		//������������
		unsigned char backbit() const;

		//������������
		Integer Abs() const;

		Integer Power(const Integer &exp) const;

	};
}

#endif