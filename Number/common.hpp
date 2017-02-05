//#include "Number.h"
namespace Number { namespace detail {
	inline void CalcTypeToSaveType(const calc_type_u CalcValue, save_type &High, save_type &Low) {
		Low = static_cast<save_type>(CalcValue & MASK_CODE);
		High = static_cast<save_type>(static_cast<calc_type_u>(CalcValue) >> BIT_NUMBER);
	}

	inline save_type FullAdder(const save_type& Num1, const save_type& Num2, save_type& c)
	{
		calc_type temp;
		temp = static_cast<calc_type>(Num1)
			+ static_cast<calc_type>(Num2)
			+ static_cast<calc_type>(c);
		save_type res;
		CalcTypeToSaveType(temp, c, res);
		return res;
	}

	//一位减法器，c=0代表借位，c=1代表不借位
	inline save_type FullSuber(const save_type& Num1, const save_type& Num2, save_type& c)
	{
		calc_type temp;
		temp = static_cast<calc_type>(Num1)
			- static_cast<calc_type>(Num2)
			+ static_cast<calc_type>(c)
			+ MODULE - 1;
		save_type res;
		CalcTypeToSaveType(temp, c, res);
		return res;
	}

	//逐位比较绝对值大小
	inline int _compare_by_digit(::std::vector<save_type>::const_reverse_iterator it1,
		::std::vector<save_type>::const_reverse_iterator it2,
		size_t digit_num, size_t& NonEqualPosition)
	{
		NonEqualPosition = 0;
		while (NonEqualPosition < digit_num) {
			if (*it1 < *it2) {
				return -1;
			}
			else if (*it1 > *it2) {
				return 1;
			}
			++it1;
			++it2;
			++NonEqualPosition;
		}
		return 0;
	}

	inline int _compare(::std::vector<save_type>::const_reverse_iterator it1,
		::std::vector<save_type>::const_reverse_iterator it2,
		size_t digit_num, exp_type exp1, exp_type exp2, char signal1,
		char signal2, size_t &NonEqualPosition) {
		//两数均为零时有符号问题，单独考虑
		if (*it1 == 0 && *it2 == 0)
			return 0;
		//符号不同直接判断
		if (signal1 != signal2)
			return static_cast<int>(signal1 - signal2);
		//位数不同直接判断
		if (exp1 != exp2)
			return signal1 * (exp1 - exp2);
		//逐位比较绝对值大小
		return signal1 * detail::_compare_by_digit(it1,
			it2, digit_num, NonEqualPosition);
	}

	inline ::std::vector<save_type> multiply_vec(const ::std::vector<save_type>& number1,
		const ::std::vector<save_type>& number2) {
		auto size1 = number1.size(), size2 = number2.size();
		::std::vector<save_type> result(size1 + size2, 0);
		for (unsigned int ii = 0;ii < size1;ii++)
		{
			for (unsigned int jj = 0;jj < size2;jj++)
			{
				calc_type_u temp = static_cast<calc_type_u>(number1[ii]) * static_cast<calc_type_u>(number2[jj]);
				save_type high, low, c = 0;
				CalcTypeToSaveType(temp, high, low);
				result[ii + jj] = FullAdder(result[ii + jj], low, c);
				result[ii + jj + 1] = FullAdder(result[ii + jj + 1], high, c);
				if (c)
					++result[ii + jj + 2];
			}
		}
		if (result.back() == 0)
			result.pop_back();
		return result;
	}

	template<typename T>
	inline void convertFloatingToInteger(T, signal_type&, ::std::vector<save_type>&, exp_type&, tolerance_type&) {
		static_assert(false, "Can't convert this type of floating point number.");
	}

	inline void convertFloatingToInteger(float number, signal_type& sig, ::std::vector<save_type>& res,
		exp_type& exp, tolerance_type& tolerance) {
		int32_t& src = reinterpret_cast<int32_t&>(number);
		calc_type_u temp = src & ((1 << 23) - 1);
		exp = (src >> 23) & ((1 << 8) - 1);
		sig = (src >> 31) ? -1 : 1;;
		if (exp == ((1 << 8) - 1))
			throw(::std::logic_error("Construct real number from NaN or Inf."));

		if (exp != 0)
			temp |= (1 << 23);
		else
			++exp;
		exp -= 127;
		exp -= 23;
		int exp_sh = (exp % (exp_type)BIT_NUMBER);
		if (exp_sh < 0)
			exp_sh += BIT_NUMBER;
		temp <<= exp_sh;
		exp = (exp - exp_sh) / (exp_type)BIT_NUMBER;
		tolerance = 1 << exp_sh;
		res.clear();
		res.push_back(temp & MASK_CODE);
		if (temp >> BIT_NUMBER != 0)
			res.push_back(temp >> BIT_NUMBER);
	}

	inline void convertFloatingToInteger(double number, signal_type& sig, ::std::vector<save_type>& res,
		exp_type& exp, tolerance_type& tolerance) {
		int64_t& src = reinterpret_cast<int64_t&>(number);
		calc_type_u temp = src & (((int64_t)1 << 52) - 1);
		exp = (src >> 52) & ((1 << 11) - 1);
		sig = (src >> 63) ? -1 : 1;
		if (exp == ((1 << 11) - 1))
			throw(::std::logic_error("Construct real number from NaN or Inf."));

		if (exp != 0)
			temp |= ((int64_t)1 << 52);
		else
			++exp;
		exp -= 1023;
		exp -= 52;
		exp_type exp_sh = (exp % (exp_type)BIT_NUMBER);
		if (exp_sh < 0)
			exp_sh += BIT_NUMBER;
		save_type temp_h = static_cast<save_type>(temp >> (BIT_NUMBER - exp_sh));
		temp <<= exp_sh;
		exp = (exp - exp_sh) / (exp_type)BIT_NUMBER;
		tolerance = 1 << exp_sh;
		res.clear();
		res.push_back(temp & MASK_CODE);
		if (temp_h != 0 || (temp >> BIT_NUMBER != 0))
			res.push_back(temp >> BIT_NUMBER);
		if (temp_h != 0)
			res.push_back(temp_h);
	}

};
};
