#pragma once
#ifndef INTEGER_H
#define INTEGER_H

#include <vector>
#include <algorithm>

using namespace std;

//所用类型的定义，根据平台不同进行改动
#define int64 __int64
#define uint64 unsigned __int64
#define int32 __int32
#define uint32 unsigned __int32

//定义存储进制
#define MODEL 4294967296 //2^32

typedef uint32 save_type;
typedef uint64 calc_type_u;
typedef int64 calc_type;

class Integer {
private:
	uint64 _exponential{ 0 };
	char _signal{ 1 };
	vector<save_type> _number{ 0 };

	Integer(vector<save_type> Number, char Signal = 1, uint64 Exponential = 0);

public:
	Integer() {}
	Integer(const Integer&);
	Integer(const int);
	Integer(const unsigned int);
	~Integer() {}

	int setSignal(const int& signal);
	//int negative();

	Integer operator- () const;
	Integer operator+(const Integer&) const;
	Integer operator- (const Integer&);


};

#endif