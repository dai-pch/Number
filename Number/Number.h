#pragma once
#ifndef _NUMBER_NUMBER_H__
#define _NUMBER_NUMBER_H__

#include <cassert>

//debug
#define DEBUG_MODE false

//�������͵Ķ��壬����ƽ̨��ͬ���иĶ�
#define int64 __int64
#define uint64 unsigned __int64
#define int32 __int32
#define uint32 unsigned __int32

typedef uint32 save_type;
typedef uint64 calc_type_u;
typedef int64 calc_type;

//����洢����
const size_t BYTE_NUMBER = sizeof(save_type);
const size_t BIT_NUMBER = (8 * sizeof(save_type));
const calc_type_u MODULE = ((calc_type_u)1 << BIT_NUMBER); //0x100000000//4294967296 //2^32
const save_type MASK_CODE = (save_type)(MODULE - 1);

inline void CalcTypeToSaveType(const calc_type_u CalcValue, save_type &High, save_type &Low) {
	Low = static_cast<save_type>(CalcValue & MASK_CODE);
	High = static_cast<save_type>(static_cast<calc_type_u>(CalcValue) >> BIT_NUMBER);
}

#endif

