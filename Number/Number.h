#pragma once
#ifndef _NUMBER_NUMBER_H__
#define _NUMBER_NUMBER_H__

#include <cassert>
#include <vector>

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
typedef int exp_type;
typedef int tolerance_type;
typedef char signal_type;

//����洢����
const size_t BYTE_NUMBER = sizeof(save_type);
const size_t BIT_NUMBER = (8 * sizeof(save_type));
const calc_type_u MODULE = ((calc_type_u)1 << BIT_NUMBER); //0x100000000//4294967296 //2^32
const save_type MASK_CODE = (save_type)(MODULE - 1);

#define	Number_Parse_Failed 0
#define	Number_Parse_OK     1

namespace Number {
	class Integer;
	class Real;

	namespace detail {
		inline void CalcTypeToSaveType(const calc_type_u CalcValue, save_type &High, save_type &Low);
		inline save_type FullAdder(const save_type& Num1, const save_type& Num2, save_type& c);
		//һλ��������c=0�����λ��c=1������λ
		inline save_type FullSuber(const save_type& Num1, const save_type& Num2, save_type& c);
		//��λ�ȽϾ���ֵ��С
		inline int _compare_by_digit(std::vector<save_type>::const_reverse_iterator it1,
			std::vector<save_type>::const_reverse_iterator it2,
			size_t digit_num, size_t& NonEqualPosition);
		inline int _compare(std::vector<save_type>::const_reverse_iterator it1,
			std::vector<save_type>::const_reverse_iterator it2,
			size_t digit_num, int exp1, int exp2, char signal1,
			char signal2, size_t &NonEqualPosition);
		inline std::vector<save_type> multiply_vec(const std::vector<save_type>& number1,
			const std::vector<save_type>& number2);
		
}} //namespace
#include "common.hpp"

#endif

