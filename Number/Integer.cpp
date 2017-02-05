#include "Integer.h"
#include <algorithm>

#define Number_Integer_Parse_Failed 0
#define Number_Integer_Parse_B 2
#define Number_Integer_Parse_O 8
#define Number_Integer_Parse_H 16
#define Number_Integer_Parse_D 4

namespace Number {

	Integer::Integer(::std::vector<save_type>& Number, char Signal) : _number(), _signal(Signal)
	{
		_number.swap(Number);
	}

	int CharToNumber(const char& ch)
	{
		if (ch >= '0' && ch <= '9')
			return (ch - '0');
		else if (ch >= 'a' && ch <= 'f')
			return (ch - 'a' + 10);
		else
			return (ch - 'A' + 10);
	}

	char NumberToChar(const unsigned int& ch)
	{
		if (ch >= 0 && ch < 10)
			return (static_cast<char>(ch) + '0');
		else if (ch >= 10 && ch <= 15)
			return (static_cast<char>(ch) + 'A' - 10);
		else
			throw ::std::logic_error("Wrong Number when convert number to char.");
	}

	void Integer::FromString10(const ::std::string &c) {
		assert(c[0] != '0' || c.length() == 1);
		auto it = c.cbegin(), end = c.cend();

		Integer result;
		for (;it != end;it++)
		{
			result = (result << 3) + (result << 1);
			result = result + CharToNumber(*it);
		}
		result._number.swap(_number);
	}

	void Integer::FromString2(const ::std::string &c)	{
		assert(c[0] != '0' || c.length() == 1);
		auto it = c.crbegin(), end = c.crend();

		::std::vector<save_type> result;
		do {
			save_type temp = 0;
			for (int jj = 0;jj < 32 && it != end;++it, ++jj)
			{
				temp += (CharToNumber(*it) << jj);
			}
			result.push_back(temp);
		} while (it != end);
		_number.swap(result);
	}

	void Integer::FromString8(const ::std::string &c)	{
		assert(c[0] != '0' || c.length() == 1);
		auto it = c.cbegin(), end = c.cend();

		Integer result;
		for (;it != end;it++)
		{
			result = result << 3;
			result = result + CharToNumber(*it);
		}
		result._number.swap(_number);
	}

	void Integer::FromString16(const ::std::string &c) {
		assert(c[0] != '0' || c.length() == 1);
		auto it = c.crbegin(), end = c.crend();

		::std::vector<save_type> result;
		do {
			save_type temp = 0;
			for (int jj = 0;jj < 8 && it != end;++it, ++jj)
			{
				temp += (CharToNumber(*it) << (4*jj));
			}
			result.push_back(temp);
		} while (it != end);
		_number.swap(result);
	}


	Integer::Integer(const Integer& src) : _number(src._number), _signal(src._signal) {}


	int Integer::setSignal(const int& signal)
	{
		return _signal = signal >= 0 ? 1 : -1;
	}

	//除去可能的高位的零
	void eraseZero(::std::vector<save_type> &vec) {
		auto it_re = vec.rbegin();
		size_t counter = 0;
		while ((it_re != vec.rend() - 1) && *it_re == 0)
		{
			++counter;
			++it_re;
		}
		vec.erase(vec.end() - counter, vec.end());
	}

	int Integer::Compare(const Integer &Obj2) const
	{
		size_t temp;
		return do_integer_compare(Obj2, temp);
	}

	::std::vector<save_type> shift_left(const ::std::vector<save_type>& src, int num) {
		assert(num >= 0);
		assert(!src.empty());
		const unsigned int num_s = num % (BIT_NUMBER);
		const unsigned int num_sr = BIT_NUMBER - num_s;
		const unsigned int num_i = num / (BIT_NUMBER);
		auto size = src.size();
		::std::vector<save_type> result(size + num_i);

		result[num_i] = src[0] << num_s;
		for (decltype(size) ii = 1; ii < size; ii++)
		{
			result[ii + num_i] = (src[ii] << num_s) | (src[ii - 1] >> num_sr);
		}
		if ((src[size - 1] >> num_sr) != 0)
			result.push_back(src[size - 1] >> num_sr);
		return result;
	}

	::std::vector<save_type> shift_right(const ::std::vector<save_type>& src, int num) {
		assert(num >= 0);
		assert(!src.empty());
		const unsigned int num_s = num % (BIT_NUMBER);
		const unsigned int num_sr = BIT_NUMBER - num_s;
		const unsigned int num_d = num / (BIT_NUMBER);
		auto size = src.size();

		if (num_d >= size)
			return ::std::vector<save_type>{0};
		::std::vector<save_type> result(size - num_d);
		for (unsigned int ii = 0;ii < size - num_d - 1;ii++)
		{
			result[ii] = (src[ii + num_d] >> num_s) | (src[ii + num_d + 1] << num_sr);
		}
		result[size - num_d - 1] = src[size - 1] >> num_s;

		if (result.back() == 0 && size != 1)
			result.pop_back();
		return result;
	}

	Integer Integer::operator<<(int num) const
	{
		::std::vector<save_type> result;
		if (num < 0)
			result = shift_right(_number, - num);
		else
			result = shift_left(_number, num);
		return Integer(result, _signal);
	}

	Integer Integer::operator>>(int num) const
	{
		::std::vector<save_type> result;
		if (num < 0)
			result = shift_left(_number, -num);
		else
			result = shift_right(_number, num);
		return Integer(result, _signal);
	}

	// Minus
	Integer Integer::operator-() const {
		return Integer(::std::vector<save_type>(_number), -_signal);
	}
	// Add
	Integer Integer::Add(const Integer& Obj2) const
	{
		//若两数符号不同，则实际为减法
		if (this->_signal != Obj2._signal)
			return (*this - (-Obj2));

		const Integer *p1, *p2;
		this->_number.size() > Obj2._number.size() ? (p1 = &Obj2, p2 = this) : (p1 = this, p2 = &Obj2);

		unsigned int size1 = p1->_number.size(), size2 = p2->_number.size();
		::std::vector<save_type> result(size2);

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

		return Integer(result, _signal);
	}

	Integer Integer::Sub(const Integer& Obj2) const	{
		//若两数符号不同，则实际为加法
		if (this->_signal != Obj2._signal)
			return (*this + (-Obj2));

		//比较两数绝对值的大小，绝对值大的赋给p1
		bool gt;
		const Integer *p1 = nullptr, *p2 = nullptr;
		size_t nonEqualCounter_re;
		int com = this->do_integer_compare(Obj2, nonEqualCounter_re);
		com *= _signal;
		if (com == 0)
			return Integer();
		gt = com > 0;
		gt ? (p1 = this, p2 = &Obj2) : (p1 = &Obj2, p2 = this);

		size_t size1 = p1->_number.size(), size2 = p2->_number.size();
		::std::vector<save_type> result(size1 - nonEqualCounter_re);
		//vector<save_type>::const_iterator it1 = p1->_number.begin(), it2 = p2->_number.begin();
		//vector<save_type>::iterator it_result = result.begin();
		calc_type temp = 0;
		save_type c = 1; //参见一位减法器说明
		unsigned int ii = 0;
		for (;ii < size2 - nonEqualCounter_re;ii++)
		{
			result[ii] = detail::FullSuber(p1->_number[ii], p2->_number[ii], c);
		}

		for (;ii < size1 - nonEqualCounter_re;ii++)
		{
			result[ii] = detail::FullSuber(p1->_number[ii], 0, c);
		}

		//除去可能的高位的零
		eraseZero(result);

		return Integer(result, gt ? _signal : -_signal);
	}

	Integer Integer::Multiply(const Integer& Obj2) const {
		unsigned int size1 = this->_number.size(), size2 = Obj2._number.size();
		::std::vector<save_type> result(size1 + size2, 0);
		// contents
		result = detail::multiply_vec(this->_number, Obj2._number);
		//signal
		char signal = this->_signal * Obj2._signal;
		return Integer(result, signal);
	}

	//除法
	void devide_Knuth(const ::std::vector<save_type>& src1, const ::std::vector<save_type>& src2,
		::std::vector<save_type>& mod, ::std::vector<save_type>& quotient) {
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
		::std::vector<save_type> number1 = shift_left(src1, shift);
		const ::std::vector<save_type> number2 = shift_left(src2, shift);

		//除法
		if (number1.back() >= number2.back())
			number1.push_back(0);
		size_t size1 = number1.size(), size2 = number2.size();
		::std::vector<save_type> result(size1 - size2);
		for (int ii = size1 - size2 - 1;ii >= 0;ii--)
		{
			//预测q
			calc_type_u qBar = number1[ii + size2] * MODULE + number1[ii + size2 - 1];
			calc_type_u rBar = qBar % number2.back();
			qBar /= number2.back(); // in this condition, qBar <= MODULE
			if (qBar == MODULE || qBar*number2[size2 - 2] > rBar * MODULE + number1[ii + size2 - 2])
				qBar--;

			//减法
			::std::vector<save_type> &tempvec = detail::multiply_vec(number2,
				::std::vector<save_type>{static_cast<save_type>(qBar)});
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
	void devide_simple(const ::std::vector<save_type>& src1, const save_type src2,
		save_type& mod, ::std::vector<save_type>& quotient) {
		calc_type_u temp = 0;
		//save_type number2 = Obj2._number[0];
		auto size1 = src1.size();
		::std::vector<save_type> result(size1);
		//::std::vector<save_type> number1 = Obj1._number;
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
	void Devide(const Integer& Obj1, const Integer& Obj2, Integer& mod, Integer& quotient)
	{
		assert(Obj2 != 0);
		//如果被除数小于除数，直接返回结果
		if (Obj1.Abs() < Obj2.Abs())
		{
			mod = Obj1;
			quotient = Integer(0);
			return;
		}

		//如果除数只有一位
		if (Obj2._number.size() < 2)
		{
			::std::vector<save_type> quotient_v;
			save_type mod_n;

			devide_simple(Obj1._number, Obj2._number[0], mod_n, quotient_v);
			mod = Integer(::std::vector<save_type>{mod_n}, Obj1._signal);
			quotient = Integer(quotient_v, Obj1._signal * Obj2._signal);
		}
		else
		{
			::std::vector<save_type> mod_v, quotient_v;

			devide_Knuth(Obj1._number, Obj2._number, mod_v, quotient_v);
			mod = Integer(mod_v, Obj1._signal);
			quotient = Integer(quotient_v, Obj1._signal * Obj2._signal);
		}
		return;
	}

	::std::ostream & operator<<(::std::ostream & os, const Integer & Obj)
	{
		// TODO: 在此处插入 return 语句
		return (os << Obj.ToString10());
	}

	::std::istream & operator>>(::std::istream &is, Integer &Num)
	{
		// TODO: 在此处插入 return 语句
		::std::string str;
		is >> str;
		Num.Parse(str);
		return is;
	}


	::std::string Integer::ToString10() const
	{
#define TO_STRING_10_BIT_ONCE 8
#define TO_STRING_MOD_ONCE (static_cast<unsigned int>(::std::pow(10, TO_STRING_10_BIT_ONCE)))

		::std::string result;
		if (_number.back() == 0)
		{
			result.push_back('0');
			return result;
		}

		Integer q = this->Abs(), mod, q_tmp;
		do
		{
			Devide(q, Integer(TO_STRING_MOD_ONCE), mod, q_tmp);
			q = q_tmp;
			save_type temp = mod._number[0];
			if (q != 0) {
				for (size_t ii = 0;ii < TO_STRING_10_BIT_ONCE;ii++) {
					result.push_back('0' + temp % 10);
					temp /= 10;
				}
			}
			else {
				size_t ii = 0;
				do {
					result.push_back('0' + temp % 10);
					temp /= 10;
				} while (ii < TO_STRING_10_BIT_ONCE && temp != 0);
			}
		} while (q != 0);
		if (_signal < 0)
			result.push_back('-');
		::std::reverse(result.begin(), result.end());
		return result;
	}

	::std::string Integer::ToString16() const
	{
		::std::string result;
		
		if (_number.back() == 0)
		{
			result= "0x0";
			return result;
		}
		
		if (_signal < 0)
			result.push_back('-');
		result.append("0x");

		auto it = _number.crbegin();
		save_type temp = *(it++);
		assert(temp != 0);
		unsigned int jj = 0;
		while (!(temp & ((save_type)15 << (BIT_NUMBER - 4))))
		{
			temp <<= 4;
			jj++;
		}
		for (;jj < 8;jj++)
		{
			result.push_back(
				NumberToChar(
					static_cast<unsigned int>(
					(temp & ((save_type)15 << (BIT_NUMBER - 4))) >> (BIT_NUMBER - 4)
						)));
			temp <<= 4;
		}
		for (;it != _number.crend();it++)
		{
			temp = *it;
			for (jj = 0;jj < 8;jj++)
			{
				result.push_back(
					NumberToChar(
						static_cast<unsigned int>(
						(temp & ((save_type)15 << (BIT_NUMBER - 4))) >> (BIT_NUMBER - 4)
							)));
				temp <<= 4;
			}
		}
		return result;
	}

#define EXPECT(it, ch) do{ \
if (*(it++) != (ch) \
	return Number_Parse_Failed; \
}

	bool IsCharB(char c) {
		return (c >= '0' && c <= '1');
	}

	bool IsCharO(char c) {
		return (c >= '0' && c <= '7');
	}

	bool IsCharD(char c) {
		return (c >= '0' && c <= '9');
	}

	bool IsCharH(char c) {
		return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
	}

	int IntegerParseNumber(::std::string::const_iterator &SrcIt, ::std::string &c, decltype(IsCharB) fun, int Ret) {
		if (!fun(*SrcIt))
			return Number_Integer_Parse_Failed;
		//跳过前导零
		while (*SrcIt == '0')
			++SrcIt;
		if (!fun(*SrcIt))
			--SrcIt;

		c.clear();
		while (fun(*SrcIt)) {
			c.push_back(*(SrcIt++));
		}
		return Ret;
	}

	int IntegerParse2(::std::string::const_iterator &SrcIt, ::std::string &c) {
		return IntegerParseNumber(SrcIt, c, IsCharB, Number_Integer_Parse_B);
	}

	int IntegerParse8(::std::string::const_iterator &SrcIt, ::std::string &c) {
		return IntegerParseNumber(SrcIt, c, IsCharO, Number_Integer_Parse_O);
	}

	int IntegerParse16(::std::string::const_iterator &SrcIt, ::std::string &c) {
		return IntegerParseNumber(SrcIt, c, IsCharH, Number_Integer_Parse_H);
	}

	int IntegerParse10(::std::string::const_iterator &SrcIt, ::std::string &c) {
		return IntegerParseNumber(SrcIt, c, IsCharD, Number_Integer_Parse_D);
	}

	int IntegerParseZero(::std::string::const_iterator &SrcIt, ::std::string &c) {
		switch (*SrcIt) {
		case 'b': case 'B': return IntegerParse2(++SrcIt, c);
		case 'o': case 'O': return IntegerParse8(++SrcIt, c);
		case 'x': case 'X': return IntegerParse16(++SrcIt, c);
		default: 
			::std::string c = "0";
			return Number_Integer_Parse_D;
		}
	}

	int IntegerParseValue(::std::string::const_iterator &SrcIt, ::std::string &c) {

		switch (*SrcIt) {
		case '0': return IntegerParseZero(++SrcIt, c);
		default:  return IntegerParse10(SrcIt, c);
		}
	}

	void IntegerParseSignal(::std::string::const_iterator &SrcIt, char& Signal) {
		Signal = 1;
		if (*SrcIt == '-')
		{
			++SrcIt;
			Signal = -1;
		}
		else if (*SrcIt == '+')
			++SrcIt;
	}

	//从字符串输入数值
	//支持2，8，10，16进制
	//词法规则：(+|-|ε)((0b num2 num2*)|(0o num8 num8*)|(0x num16 num16*)|(num10 num10*))
	int Integer::Parse(::std::string str)
	{
		str.push_back('\0');
		auto it = str.begin();
		::std::string c;
		//处理符号
		IntegerParseSignal(it, _signal);

		int res = IntegerParseValue(it, c);

		if (*it != '\0')
			return Number_Parse_Failed;

		switch (res) {
		case Number_Integer_Parse_B:  this->FromString2(c);  break;
		case Number_Integer_Parse_O:  this->FromString8(c);  break;
		case Number_Integer_Parse_D:  this->FromString10(c); break;
		case Number_Integer_Parse_H:  this->FromString16(c); break;
		case Number_Integer_Parse_Failed: default:
			return Number_Parse_Failed;
		}
		return Number_Parse_OK;
	}

	size_t Integer::get_digit_number() const
	{
		return _number.size();
	}

	int Integer::do_integer_compare(const Integer &Obj2, size_t &NonEqualPosition) const
	{
		size_t size1 = _number.size(), size2 = Obj2._number.size();
		return detail::_compare(_number.rbegin(), Obj2._number.rbegin(),
			size1, size1, size2, _signal, Obj2._signal, NonEqualPosition);
	}

	unsigned char Integer::backbit() const
	{
		return static_cast<unsigned char>(_number[0] & 1);
	}

	Integer Integer::Abs() const
	{
		return Integer(::std::vector<save_type>(_number), 1);
	}

	Integer Integer::Power(const Integer & Exp) const
	{
		Integer result(1);
		Integer temp(*this), exp(Exp);
		unsigned char flag = exp.backbit();
		if (flag)
			result = result * temp;
		exp = exp >> 1;
		while (exp != 0)
		{
			if (DEBUG_MODE)
				::std::cout << result << ::std::endl;
			temp = temp * temp;
			flag = exp.backbit();
			if (flag)
				result = result * temp;
			exp = exp >> 1;
		}
		return result;
	}

}
