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

	Real::Real(float Number, size_t precision)
	{
		detail::convertFloatingToInteger(Number, _signal, _number._number, _exp);
		SetPrecision(precision);
	}

	Real::Real(double number, size_t precision)
	{
		detail::convertFloatingToInteger(number, _signal, _number._number, _exp);
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

	Real & Real::operator=(const Integer &number)
	{
		_number = number._number;
		_signal = number._signal;
		_exp = 0;
		SetPrecision(default_precision);
		return *this;
	}

	Real & Real::operator=(Integer &&number)
	{
		_number = ::std::forward<UInteger>(number._number);
		_signal = number._signal;
		_exp = 0;
		SetPrecision(default_precision);
		return *this;
	}

	Real & Real::operator=(const UInteger &number)
	{
		_number = number;
		_signal = 1;
		_exp = 0;
		SetPrecision(default_precision);
		return *this;
	}

	Real & Real::operator=(UInteger &&number)
	{
		_number = ::std::forward<UInteger>(number);
		_signal = 1;
		_exp = 0;
		SetPrecision(default_precision);
		return *this;
	}

	Real & Real::operator=(float src)
	{
		detail::convertFloatingToInteger(src, _signal, _number._number, _exp);
		SetPrecision(default_precision);
		return *this;
	}

	Real & Real::operator=(double src)
	{
		detail::convertFloatingToInteger(src, _signal, _number._number, _exp);
		SetPrecision(default_precision);
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

	Real Real::operator-() const
	{
		return Real(_numvec, -1);
	}


	::std::string Real::ToString10() const
	{
		return ::std::string();
	}

	void Real::RealParseF(::std::string::const_iterator& it,
		const ::std::string::const_iterator end, UInteger& f,
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
		it = range;
		//::std::cout << f_dec << ::std::endl;
		if (f.Parse(f_dec) == Number_Parse_Failed)
			throw ::std::runtime_error("Parse error in number part.");
	}

	void Real::RealParseExp(::std::string::const_iterator& it,
		const ::std::string::const_iterator end, exp_type& e) {
		if (*it == 'e' || *it == 'E')
		{
			::std::string exp_dec;
			auto range = ::std::find_if_not(++it, end,
				[](char c) {return c >= '0' && c <= '9';});
			if (range != end) // if the exponent part contains invalid char.
				throw ::std::runtime_error("Invalid charactor in exponential part.");
			std::copy(it, end, std::back_inserter(exp_dec));
			Integer temp;
			if (temp.Parse(exp_dec) == Number_Parse_Failed)
				throw ::std::runtime_error("Invalid charactor in number part.");
			if (temp.size() > 1)
				throw ::std::runtime_error("Overflow or underflow.");
			e += temp._signal * temp._numvec[0];
		}
		else if (it == end)
			return;
		else
			throw ::std::runtime_error("Invalid format of floating point number.");
	}

	// Reference: 
	// Clinger W D. How to read floating point numbers accurately[J].
	// programming language design and implementation, 1990, 25(6): 92-101.
	inline void AlgorithmM(const UInteger& f, const exp_type e, size_t prec, UInteger& m, exp_type& k) {
		UInteger u, v;
		k = 0;
		if (e < 0) {
			u = f;
			v = UInteger((unsigned)10).Power(e);
		}
		else {
			u = f * UInteger((unsigned)10).Power(e);
			v = (unsigned)1;
		}
		// loop(u v k)
		UInteger x, r;
		Devide(u, v, r, x);
		int diff = x.size() - prec;
		if (diff >= 1) {
			v = v << (diff*BIT_NUMBER);
		}
		else if (diff < 0) {
			u = u << ((-diff)*BIT_NUMBER);
		}
		k += diff;
		Devide(u, v, r, x);
		assert(x.size() == prec);

		if ((r << 1) < v) // err < 1/2 * precision
			m = x;
		else if ((r << 1) > v) // err > 1/2 * precision
			m = x + (unsigned)1;
		else if (IsEven(x)) // err == 1/2 * precision
			m = x;
		else
			m = x + (unsigned)1;
	}

	int Real::Parse(const ::std::string& str)
	{
		auto it = str.begin(), end = str.end();
		UInteger f;
		exp_type e;
		int res = Number_Parse_OK;
		auto prec = _number.size();
		
		try {
			detail::NumberParseSignal(it, _signal);
			RealParseF(it, end, f, e);
			RealParseExp(it, end, e);
			AlgorithmM(f, e, prec, _number, _exp);
			Normalize(prec);
		}
		catch (std::exception) {
			res = Number_Parse_Failed;
		}
		return res;
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
