#include "Integer.h"
#include "Real.h"
#include <algorithm>
#include <iterator>
#include <exception>

namespace Number {

	size_t Real::default_precision = 96;

	Real::Real(const Integer& Obj): _number(Obj._number), _signal(Obj._signal) {}

	int Real::Compare(const Real& number2) const
	{
		size_t temp;
		exp_type exp1 = _number.size() + _exp;
		exp_type exp2 = number2._number.size()
			+ number2._exp;
		size_t digit_num = ::std::min(_number.size(), number2._number.size());
		auto ret = detail::_compare(_number.rbegin(), number2._number.rbegin(),
			digit_num, exp1, exp2, _signal, number2._signal, temp);
		if (ret == 0 && temp == digit_num)
			return _number.size() - number2._number.size();
		else
			return ret;
		return 0;
	}


	::std::string Real::ToString10() const
	{
		return ::std::string();
	}

	void Real::RealParseF(::std::string::const_iterator it,
		::std::string::const_iterator end, ::std::vector<save_type>& f,
		exp_type& e) {
		::std::string f_dec;
		auto range = ::std::find_if_not(it, end,
			[](char c) {return c >= '0' && c <= '9';});
		::std::copy(it, range, ::std::back_inserter(f_dec));
		it = range + 1;
		if (*range == '.') {
			range = ::std::find_if_not(it, end,
				[](char c) {return c >= '0' && c <= '9';});
			::std::copy(it, range, ::std::back_inserter(f_dec));
		}
		e = -(range - it);
		//::std::cout << f_dec << ::std::endl;
		Integer temp;
		if (temp.Parse(f_dec) == Number_Parse_Failed)
			throw ::std::runtime_error("Parse error in number part.");
		f.swap(temp._number);
	}

	void Real::RealParseExp(::std::string::const_iterator it,
		::std::string::const_iterator end, exp_type& e) {
		if (*it == 'e' || *it == 'E')
		{
			::std::string exp_dec;
			auto range = ::std::find_if_not(++it, end,
				[](char c) {return c >= '0' && c <= '9';});
			if (range != end) // if the exponent part contains invalid char.
				throw ::std::runtime_error("Invalid charactor in exponential part.");
			Integer temp;
			if (temp.Parse(exp_dec) == Number_Parse_Failed)
				throw ::std::runtime_error("Invalid charactor in number part.");
			if (temp.get_digit_number() > 1)
				throw ::std::runtime_error("Overflow or underflow.");
			e += temp._signal * temp.get_digit_number[0];
		}
		else if (it == end)
			return;
		else
			throw ::std::runtime_error("Invalid format of floating point number.");
	}

	inline void AlgorithmM(::std::vector<save_type> f, exp_type k, ::std::vector<save_type>& m, exp_type& e) {

	}

	int Real::Parse(const ::std::string& str)
	{
		auto it = str.begin(), end = str.end();
		::std::vector<save_type> f;
		exp_type e;
		int res = Number_Parse_OK;
		
		try {
			detail::NumberParseSignal(it, _signal);
			RealParseF(it, end, f, e);
			RealParseExp(it, end, e);
			AlgorithmM(f, e, _number, _exp);
		}
		catch (std::exception e) {
			res = Number_Parse_Failed;
		}
		return res;
	}

	void Real::SetDefaultPrecision(size_t pre)
	{
		default_precision = pre;
	}

	size_t Real::GetDefaultPrecision()
	{
		return default_precision;
	}

	void Real::SetPrecision(size_t precision)
	{
		precision = (precision - 1) / BIT_NUMBER + 2;
		if (_number.size() > precision) {
			auto it = _number.begin() + (_number.size() - precision);
			_number.erase(_number.begin(), it);
		}
		else 
			while(_number.size() < precision) {
				_number.push_back(0);
			}
	}

	size_t Real::GetPrecision()
	{
		return _number.size() - 1;
	}

}
