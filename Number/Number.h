#pragma once
#ifndef _NUMBER_NUMBER_H__
#define _NUMBER_NUMBER_H__

//debug
#define DEBUG_MODE false

//所用类型的定义，根据平台不同进行改动
#define int64 __int64
#define uint64 unsigned __int64
#define int32 __int32
#define uint32 unsigned __int32

typedef uint32 save_type;
typedef uint64 calc_type_u;
typedef int64 calc_type;

//定义存储进制
#define BYTE_NUMBER sizeof(save_type)
#define BIT_NUMBER (8 * sizeof(save_type))
#define MODULE (1 << BIT_NUMBER)//0x100000000//4294967296 //2^32

#endif

