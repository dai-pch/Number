#pragma once
#ifndef INTEGER_H
#define INTEGER_H

#include <vector>
#include <algorithm>
#include <type_traits>

using namespace std;

//�������͵Ķ��壬����ƽ̨��ͬ���иĶ�
#define int64 __int64
#define uint64 unsigned __int64
#define int32 __int32
#define uint32 unsigned __int32

//����洢����
#define MODULE 0x100000000//4294967296 //2^32

typedef uint32 save_type;
typedef uint64 calc_type_u;
typedef int64 calc_type;

class Integer {
private:
	char _signal = 1;
	vector<save_type> _number{ 0 };

	Integer(vector<save_type> Number, char Signal = 1);


public:
	Integer() {}
	Integer(const Integer&);

	template<typename T>
	Integer(const T& Source,
		typename std::enable_if<std::is_integral<T>::value>::type* = nullptr)
	{
		T temp = Source < 0 ? (_signal = -1, -Source) : (_signal = 1, Source);
		_number.reserve(sizeof(T) / sizeof(save_type) + 1);
		do{
			_number.push_back(temp % MODULE);
			temp /= MODULE;
		} while (temp != 0);
	}

	//Integer(const int);
	//Integer(const unsigned int);
	~Integer() {}


	int setSignal(const int& signal);
	//int negative();


	//�Ƚ�����
	//>�����
	bool operator>(const Integer&) const;
	template<typename T>
	friend inline bool operator>(const T& Obj1, const Integer& Obj2)
	{
		return Obj2 < Obj1;
	}
	//<�����
	bool operator<(const Integer&) const;
	template<typename T>
	friend inline bool operator<(const T& Obj1, const Integer& Obj2)// const
	{
		return Obj2 > Obj1;
	}
	//==�����
	bool operator==(const Integer&) const;
	template<typename T>
	friend inline bool operator==(const T& Obj1, const Integer& Obj2)
	{
		return Obj2 == Obj1;
	}
	//>=�����
	inline bool operator>=(const Integer& Number2)
	{
		return !(*this < Number2);
	}
	template<typename T>
	friend inline bool operator>=(const T& Number1, const Integer& Number2)
	{
		return !(Number2 > Number1);
	}
	//<=�����
	inline bool operator<=(const Integer& Number2)
	{
		return !(*this > Number2);
	}
	template<typename T>
	friend inline bool operator<=(const T& Number1, const Integer& Number2)
	{
		return !(Number2 < Number1);
	}

	Integer operator-() const;
	Integer operator+(const Integer&) const;
	template<typename T>
	friend inline Integer operator+(const T& Number1, const Integer& Number2)
	{
		return Number2 + Number1;
	}
	Integer operator-(const Integer&) const;
	template<typename T>
	friend inline Integer operator-(const T& Number1, const Integer& Number2)
	{
		return (-Number2) + Number1;
	}




	

};

#endif