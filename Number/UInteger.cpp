#include "UInteger.h"
#include <algorithm>
#include <type_traits>
#include <iterator>

using ::std::vector;
namespace Number {
	UInteger::UInteger(const UInteger& src) : _number(src._number) {}

	UInteger::UInteger(const vector<save_type>& src) : _number(src) {}
	UInteger::UInteger(vector<save_type>&& src) 
		: _number(::std::forward<vector<save_type>>(src)) {}

	int UInteger::Compare(const UInteger &Obj2) const
	{
		size_t size1 = _number.size(), size2 = Obj2._number.size(), temp;
		//位数不同直接判断
		if (size1 != size2)
			return (size1 - size2);
		return detail::_compare_by_digit(this->_number.crbegin(), Obj2._number.crbegin(), size1, temp);
	}

	vector<save_type> shift_left(const vector<save_type>& src, int num) {
		assert(num >= 0);
		assert(!src.empty());
		if (src.back() == 0 && src.size() == 1)
			return vector<save_type>(src);
		const unsigned int num_s = num % (BIT_NUMBER);
		const unsigned int num_sr = BIT_NUMBER - num_s;
		const unsigned int num_i = num / (BIT_NUMBER);
		auto size = src.size();
		vector<save_type> result(size + num_i);

		result[num_i] = src[0] << num_s;
		for (decltype(size) ii = 1; ii < size; ii++)
		{
			result[ii + num_i] = (src[ii] << num_s) | (src[ii - 1] >> num_sr);
		}
		if ((src[size - 1] >> num_sr) != 0)
			result.push_back(src[size - 1] >> num_sr);
		return result;
	}

	vector<save_type> shift_right(const vector<save_type>& src, int num) {
		assert(num >= 0);
		assert(!src.empty());
		const unsigned int num_s = num % (BIT_NUMBER);
		const unsigned int num_sr = BIT_NUMBER - num_s;
		const unsigned int num_d = num / (BIT_NUMBER);
		auto size = src.size();

		if (num_d >= size)
			return vector<save_type>{0};
		vector<save_type> result(size - num_d);
		for (unsigned int ii = 0;ii < size - num_d - 1;ii++)
		{
			result[ii] = (src[ii + num_d] >> num_s) | (src[ii + num_d + 1] << num_sr);
		}
		result[size - num_d - 1] = src[size - 1] >> num_s;

		if (result.back() == 0 && size != 1)
			result.pop_back();
		return result;
	}

	UInteger UInteger::operator<<(int num) const
	{
		vector<save_type> result;
		if (num < 0)
			result = shift_right(_number, -num);
		else
			result = shift_left(_number, num);
		return UInteger(result);
	}

	UInteger UInteger::operator>>(int num) const
	{
		vector<save_type> result;
		if (num < 0)
			result = shift_left(_number, -num);
		else
			result = shift_right(_number, num);
		return UInteger(result);
	}

	//除去可能的高位的零
	void eraseZero(vector<save_type> &vec) {
		auto it_re = vec.rbegin();
		size_t counter = 0;
		while ((it_re != vec.rend() - 1) && *it_re == 0)
		{
			++counter;
			++it_re;
		}
		vec.erase(vec.end() - counter, vec.end());
	}

	UInteger UInteger::Add(const UInteger& Obj2) const
	{
		const UInteger *p1, *p2;
		this->_number.size() > Obj2._number.size() ? (p1 = &Obj2, p2 = this) : (p1 = this, p2 = &Obj2);

		unsigned int size1 = p1->_number.size(), size2 = p2->_number.size();
		vector<save_type> result(size2);

		calc_type temp = 0;
		save_type c = 0;
		unsigned int ii = 0;
		for (;ii < size1;ii++)
		{
			result[ii] = detail::FullAdder(p1->_number[ii], p2->_number[ii], c);
		}

		for (;ii < size2;ii++)
		{
			result[ii] = detail::FullAdder(0, p2->_number[ii], c);
		}

		if (c != 0)
			result.push_back(c);

		return UInteger(result);
	}

	UInteger UInteger::Sub(const UInteger& Obj2) const {
		assert(*this >= Obj2);
		const UInteger* p1 = this;
		const UInteger* p2 = &Obj2;

		size_t size1 = p1->_number.size(), size2 = p2->_number.size();
		vector<save_type> result;
		calc_type temp = 0;
		save_type c = 1; //参见一位减法器说明
		size_t ii = 0;
		auto re_it = ::std::back_inserter(result);
		for (;ii < size2;ii++)
		{
			*re_it = detail::FullSuber(p1->_number[ii], p2->_number[ii], c);
		}

		for (;ii < size1;ii++)
		{
			*re_it = detail::FullSuber(p1->_number[ii], 0, c);
		}

		//除去可能的高位的零
		eraseZero(result);

		return UInteger(result);
	}

	UInteger UInteger::Multiply(const UInteger& Obj2) const {
		unsigned int size1 = this->_number.size(), size2 = Obj2._number.size();
		auto& number1 = this->_number;
		auto& number2 = Obj2._number;
		vector<save_type> result(size1 + size2, 0);
		for (unsigned int ii = 0;ii < size1;ii++)
		{
			for (unsigned int jj = 0;jj < size2;jj++)
			{
				calc_type_u temp = static_cast<calc_type_u>(number1[ii]) * static_cast<calc_type_u>(number2[jj]);
				save_type high, low, c = 0;
				detail::CalcTypeToSaveType(temp, high, low);
				result[ii + jj] = detail::FullAdder(result[ii + jj], low, c);
				result[ii + jj + 1] = detail::FullAdder(result[ii + jj + 1], high, c);
				if (c)
					++result[ii + jj + 2];
			}
		}
		if (result.back() == 0)
			result.pop_back();
		return UInteger(result);
	}

	//除法
	void devide_Knuth(const vector<save_type>& src1, const vector<save_type>& src2,
		vector<save_type>& mod, vector<save_type>& quotient) {
		assert(src1.size() > src2.size() ||
			(src1.size() == src2.size() && src1.back() >= src2.back()));

		//规格化
		save_type temp = src2.back();
		int shift = 31;
		if ((temp & 0xffff0000) != 0) { shift -= 16; temp &= 0xffff0000; }
		if ((temp & 0xff00ff00) != 0) { shift -= 8; temp &= 0xff00ff00; }
		if ((temp & 0xf0f0f0f0) != 0) { shift -= 4; temp &= 0xf0f0f0f0; }
		if ((temp & 0xcccccccc) != 0) { shift -= 2; temp &= 0xcccccccc; }
		if ((temp & 0xaaaaaaaa) != 0) { shift -= 1; }
		vector<save_type> number1 = shift_left(src1, shift);
		const vector<save_type> number2 = shift_left(src2, shift);

		//除法
		if (number1.back() >= number2.back())
			number1.push_back(0);
		size_t size1 = number1.size(), size2 = number2.size();
		vector<save_type> result(size1 - size2);
		for (int ii = size1 - size2 - 1;ii >= 0;ii--)
		{
			//预测q
			calc_type_u qBar = number1[ii + size2] * MODULE + number1[ii + size2 - 1];
			calc_type_u rBar = qBar % number2.back();
			qBar /= number2.back(); // in this condition, qBar <= MODULE
			if (qBar == MODULE || qBar*number2[size2 - 2] > rBar * MODULE + number1[ii + size2 - 2])
				qBar--;

			//减法
			vector<save_type> &tempvec = detail::multiply_vec(number2,
				vector<save_type>{static_cast<save_type>(qBar)});
			if (tempvec.size() == size2)
				tempvec.push_back(0);
			save_type c = 1;
			for (decltype(tempvec.size()) jj = 0;jj < tempvec.size();jj++)
				number1[jj + ii] = detail::FullSuber(number1[jj + ii], tempvec[jj], c);

			//如果预测不正确
			if (c == 0)
			{
				for (decltype(tempvec.size()) jj = 0;jj <= tempvec.size();jj++)
					number1[jj + ii] = detail::FullAdder(number1[jj + ii], number2[jj], c);
				qBar--;
			}
			result[ii] = static_cast<save_type>(qBar);
		}

		//除去可能的高位的零
		eraseZero(number1);
		mod = shift_right(number1, shift);
		quotient = result;
	}

	void devide_simple(const vector<save_type>& src1, const save_type src2,
		save_type& mod, vector<save_type>& quotient) {
		calc_type_u temp = 0;
		//save_type number2 = Obj2._number[0];
		auto size1 = src1.size();
		vector<save_type> result(size1);
		//vector<save_type> number1 = Obj1._number;
		//temp = 0;
		for (int ii = size1 - 1;ii >= 0;ii--)
		{
			temp *= MODULE;
			temp += src1[ii];
			result[ii] = static_cast<save_type>(temp / src2);
			temp = static_cast<calc_type_u>(temp % src2);
		}
		if (result.back() == 0 && result.size() > 1)
			result.pop_back();

		mod = static_cast<save_type>(temp);
		quotient = result;
	}

	//使用Knuth算法
	void Devide(const UInteger& Obj1, const UInteger& Obj2, UInteger& mod, UInteger& quotient)
	{
		assert(Obj2 != (unsigned)0);
		//如果被除数小于除数，直接返回结果
		if (Obj1 < Obj2)
		{
			mod = Obj1;
			quotient = UInteger((unsigned)0);
			return;
		}

		//如果除数只有一位
		if (Obj2._number.size() < 2)
		{
			vector<save_type> quotient_v;
			save_type mod_n;

			devide_simple(Obj1._number, Obj2._number[0], mod_n, quotient_v);
			mod = UInteger(vector<save_type>{mod_n});
			quotient = UInteger(quotient_v);
		}
		else {
			vector<save_type> mod_v, quotient_v;

			devide_Knuth(Obj1._number, Obj2._number, mod_v, quotient_v);
			mod = UInteger(mod_v);
			quotient = UInteger(quotient_v);
		}
		return;
	}

	void _power(UInteger& result, UInteger& base, save_type exp) {
		for (unsigned ii = 0;ii < BIT_NUMBER;++ii) {
			if ((exp & 1) == 1) {
				result = result * base;
			}
			base = base * base;
			exp >>= 1;
		}
		assert(exp == 0);
	}

	UInteger UInteger::Power(const save_type exp) const
	{
		UInteger result((unsigned)1);
		UInteger base(_number);
		_power(result, base, exp);
		return UInteger(result);
	}

	UInteger UInteger::Power(const UInteger & exp) const
	{
		UInteger result((unsigned)1), base(_number);
		for (save_type ele : exp._number) {
			_power(result, base, ele);
		}
		return UInteger(result);
	}

} // namespace Number