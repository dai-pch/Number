#include "Integer.h"
#include "Real.h"
#include <algorithm>
#include <iterator>
#include <exception>

using ::std::vector;

namespace Number {

	size_t Real::default_precision = 96;

	Real::Real(const Integer& Obj, size_t precision)
		: _number(Obj._number._number), _signal(Obj._signal) {
		SetPrecision(precision);
	}

	Real::Real(const UInteger& Obj, size_t precision)
		: _number(Obj._number), _signal(1) {
		SetPrecision(precision);
	}

	Real & Real::operator=(const Real &src)
	{
		_number = (src._number);
		_signal = src._signal;
		_exp = src._exp;
		return *this;
	}

	Real &Real::operator=(Real &&src)
	{
		_number = ::std::forward<UInteger>(src._number);
		_signal = src._signal;
		_exp = src._exp;
		return *this;
	}

	int Real::Compare(const Real& number2) const
	{
		exp_type exp1 = _number.size() + _exp;
		exp_type exp2 = number2._number.size()
			+ number2._exp;
		//两数有一个为零时有符号问题，单独考虑
		if (_numvec.back() == 0) {
			if (number2._numvec.back() == 0)
				return 0;
			else
				return -number2._signal;
		}
		else if (number2._numvec.back() == 0)
			return _signal;
		// 符号不同直接判断
		if (_signal != number2._signal)
			return _signal - number2._signal;
		//指数不同直接判断
		if (exp1 != exp2)
			return _signal * (exp1 - exp2);
		int res = detail::_compare_by_digit(_numvec.crbegin(),
			number2._numvec.crbegin(), _number.size());
		return _signal < 0 ? -res : res;
	}


	::std::string Real::ToString10() const
	{
		return ::std::string();
	}

	void Real::RealParseF(::std::string::const_iterator it,
		const ::std::string::const_iterator end, vector<save_type>& f,
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
		f.swap(temp._number._number);
	}

	void Real::RealParseExp(::std::string::const_iterator it,
		const ::std::string::const_iterator end, exp_type& e) {
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
			if (temp.size() > 1)
				throw ::std::runtime_error("Overflow or underflow.");
			e += temp._signal * temp.size();
		}
		else if (it == end)
			return;
		else
			throw ::std::runtime_error("Invalid format of floating point number.");
	}

	void Real::Normalize(size_t precision)
	{
		int diff = _number.size() - precision;
		if (diff == 0)
			return;
		else if (diff > 0) {
			_numvec.erase(_numvec.cbegin(), _numvec.cbegin() + diff);
		}
		else {
			vector<save_type> temp(precision);
			std::copy(_numvec.crbegin(), _numvec.crend(), temp.rbegin());
			_numvec.swap(temp);
		}
		if (_numvec.back() != 0)
			_exp += diff;
		else
			_exp = 0;
	}

	inline void AlgorithmM(const UInteger& f, const exp_type e, UInteger& m, exp_type& k) {
		UInteger u, v;
		if (e < 0) {
			u = f;
			
		}
	}

	int Real::Parse(const ::std::string& str)
	{
		auto it = str.begin(), end = str.end();
		vector<save_type> f;
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
		Normalize(precision);
	}

	size_t Real::GetPrecision()
	{
		return _number.size() - 1;
	}

}
