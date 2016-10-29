#pragma once
#ifndef _NUMBER_REAL_H__
#define _NUMBER_REAL_H__

#include "Number.h"
#include "Integer.h"

namespace Number {
	class Real {
	public:
		//构造函数
		Real(){} //空构造
		Real(Real&); //拷贝构造
		Real(Integer&); 
		template<typename T>
		Real(T& Number, typename std::enable_if<
		std::is_floating_point<T>::value>::type* = nullptr) { //从浮点数构造

		}

		//比较函数
		int Compare(const Real&) const;
		//>运算符
		bool operator>(const Real &Number2) const {
			return ((this->Compare(Number2)) > 0);
		}
		template<typename T>
		friend bool operator>(const T& Number1, const Real& Number2)
		{
			return Number2 < Number1;
		}
		//<运算符	
		bool operator<(const Real& Number2) const {
			return ((this->Compare(Number2)) < 0);
		}
		template<typename T>
		friend bool operator<(const T& Number1, const Real& Number2)
		{
			return Number2 > Number1;
		}
		//==运算符
		bool operator==(const Real& Number2) const {
			return ((this->Compare(Number2)) == 0);
		}
		template<typename T>
		friend bool operator==(const T& Number1, const Real& Number2)
		{
			return Number2 == Number1;
		}
		//!=运算符
		bool operator!=(const Real& Number2) const
		{
			return ((this->Compare(Number2)) != 0);
		}
		template<typename T>
		friend bool operator!=(const T& Number1, const Real& Number2)
		{
			return (Number2 != Number1);
		}
		//>=运算符
		bool operator>=(const Real& Number2) const
		{
			return ((this->Compare(Number2)) >= 0);
		}
		template<typename T>
		friend bool operator>=(const T& Number1, const Real& Number2)
		{
			return (Number2 <= Number1);
		}
		//<=运算符
		bool operator<=(const Real& Number2) const
		{
			return ((this->Compare(Number2)) <= 0);
		}
		template<typename T>
		friend bool operator<=(const T& Number1, const Real& Number2)
		{
			return (Number2 >= Number1);
		}

		//算数操作符
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

		//转换为字符串
		::std::string ToString10() const;
		::std::string ToString16() const;
		int Parse(std::string);

		//输入输出
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
