#include "UInteger.h"
#include "Integer.h"
#include "Real.h"
#include <algorithm>
#include <iterator>
#include <exception>
#include <cctype>

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
			number2._numvec.crbegin(), _number.size(), number2.size());
		return _signal < 0 ? -res : res;
	}

	void _add(const vector<save_type>& a, const vector<save_type>& b, const exp_type exp_a, const exp_type exp_b,
		vector<save_type>& res, exp_type& exp_res) {
		exp_type s_a = a.size(), s_b = b.size();
		exp_type h_a = s_a + exp_a, h_b = s_b + exp_b;
		// The highest bit of one number is smaller than the lowest bit of another;
		if (h_a < exp_b) {
			res = b;
			exp_res = exp_b;
		}
		else if (h_b < exp_a) {
			res = a;
			exp_res = exp_a;
		}
		else //else
		{
			exp_res = std::min(exp_a, exp_b);
			exp_type len = std::max(h_a, h_b) - exp_res;
			vector<save_type> result(len, 0);
			save_type c = 0;
			for (exp_type ii = 0;ii < len;++ii) {
				result[ii] = detail::FullAdder(
					ii >= exp_a - exp_res && ii < h_a - exp_res ? a[ii + exp_res - exp_a] : 0,
					ii >= exp_b - exp_res && ii < h_b - exp_res ? b[ii + exp_res - exp_b] : 0,
					c);
			}
			if (c != 0)
				result.push_back(c);
			res = result;
		}
	}
	
	// assert a >= b
	void _sub(const vector<save_type>& a, const vector<save_type>& b, const exp_type exp_a, const exp_type exp_b,
		vector<save_type>& res, exp_type& exp_res) {
		exp_type h_a = a.size() + exp_a, h_b = b.size() + exp_b;
		assert(h_a >= h_b);
		// The highest bit of one number is smaller than the lowest bit of another;
		if (h_b < exp_a) {
			res = a;
			exp_res = exp_a;
		}
		else //else
		{
			exp_res = std::min(exp_a, exp_b);
			exp_type len = h_a - exp_res;
			vector<save_type> result(len, 0);
			save_type c = 1;
			for (exp_type ii = 0;ii < len;++ii) {
				result[ii] = detail::FullSuber(
					ii + exp_res >= exp_a && ii + exp_res < h_a ? a[ii + exp_res - exp_a] : 0,
					ii + exp_res >= exp_b && ii + exp_res < h_b ? b[ii + exp_res - exp_b] : 0,
					c);
			}
			detail::eraseZero(result);
			res = result;
		}
	}

	Real Real::operator-() const
	{
		return Real(_numvec, -1);
	}

	Real Real::Add(const Real & num) const
	{
		if (_signal != num._signal)
			return this->Sub(-num);
		else
		{
			exp_type exp_res;
			vector<save_type> res;
			_add(_numvec, num._numvec, _exp, num._exp, res, exp_res);
			Real r(res);
			r.RoundTo(std::max(_numvec.size(), num._numvec.size()));
			return r;
		}
	}

	Real Real::Sub(const Real & num) const
	{
		if (_signal != num._signal) {
			exp_type exp_res;
			vector<save_type> res;
			_add(_numvec, num._numvec, _exp, num._exp, res, exp_res);
			Real r(res, _signal);
			r.RoundTo(std::max(_numvec.size(), num._numvec.size()));
			return r;
		}
		else
		{
			exp_type exp_res;
			vector<save_type> res;
			char sig;
			if (_number >= num._number) {
				_sub(_numvec, num._numvec, _exp, num._exp, res, exp_res);
				sig = _signal;
			}
			else {
				_sub(num._numvec, _numvec, num._exp, _exp, res, exp_res);
				sig = -_signal;
			}
			Real r(res, sig);
			r.RoundTo(std::max(_numvec.size(), num._numvec.size()));
			return r;
		}
	}

	Real Real::Multiply(const Real & num) const
	{
		Real res;
		res._number = _number * num._number;
		res._exp = _exp + num._exp;
		res._signal = _signal == num._signal ? 1 : -1;
		res.RoundTo(std::min(size(), num.size()));
		return res;
	}

	Real Devide(const Real &n1, const Real &n2)
	{
		Real res;
		UInteger mod;
		size_t prec = std::min(n1.size(), n2.size());
		size_t diff = n2.size() + prec - n1.size();
		Devide((n1._number << (diff * BIT_NUMBER)), n2._number, mod, res._number);
		if (((mod << 1) > n2._number)
			|| ((mod << 1)==n2._number && IsEven(res._number)))
			res._number = res._number + (unsigned)1;
		res._exp = n1._exp - n2._exp - diff;
		res._signal = (n1._signal == n2._signal ? 1 : -1);
		res.RoundTo(std::min(n1.size(), n2.size()));
		return res;
	}

	::std::ostream & operator<<(::std::ostream &os, const Real &num)
	{
		return os << num.ToString10();
	}

	::std::istream & operator >> (::std::istream &os, Real &num)
	{
		std::string tem;
		os >> tem;
		num.Parse(tem.c_str());
		return os;
	}

	// Algorthm comes from:
	// Burger R G, Dybvig R K. Printing floating-point numbers
	// quickly and accurately[J]. Acm Sigplan Notices, 1996, 31(5):108-116.
	void _to_radix_10(const UInteger& f, const exp_type e, std::string& m, exp_type k = 0) {
		// initialize
		size_t p = f.size();
		bool even = IsEven(f);
		UInteger b_p1((unsigned)1),b_e((unsigned)1), r, s, m_u, m_d;
		b_p1 = b_p1 << (p*BIT_NUMBER);
		b_e = b_e << (e*BIT_NUMBER);
		if (e >= 0) {
			r = (f * b_e) << 1; // r = 2 * f * b^e
			s = (unsigned)2; // s = 2
			m_d = m_u = b_e; // m_d = m_u = b^e
			if (f == b_p1) {
				r = r << BIT_NUMBER; // r = 2 * f * b^(e+1)
				s = s << BIT_NUMBER; // s = 2 * b
				m_u = m_u << BIT_NUMBER; // m_u = b^(e+1)
			}
		}
		else {
			r = f << 1; // r = 2 * f
			s = (unsigned)2; 
			s = s << (-e * BIT_NUMBER); // s = 2 * b^(-e)
			m_d = m_u = (unsigned)1; // m_d = m_u = 1
			if (f == b_p1) {
				r = r << BIT_NUMBER; // r = 2 * f * b
				s = s << BIT_NUMBER; // s = 2 * b^(-e+1)
				m_u = m_u << BIT_NUMBER; // m_u = b
			}
		}

		// scale
		UInteger B((unsigned)10);
		// using k approxmation
		
		if (k > 0)
			s = s * B.Power(k);
		else if (k < 0) {
			UInteger B_k = B.Power(-k);
			r = r * B_k;
			m_u = m_u * B_k;
			m_d = m_d * B_k;
		}
		// fix
		while (1) {
			if (r + m_u >= s) {
				s = s * B;
				++k;
			}
			else if ((r + m_u)*B < s) {
				r = r * B;
				m_u = m_u * B;
				m_d = m_d * B;
				--k;
			}
			else
				break;
		}

		//generate
		while (1) {
			UInteger mod;
			Devide(r * B, s, r, mod);
			char d = (save_type)mod + '0';
			m_u = m_u * B;
			m_d = m_d * B;
			bool cond1 = r < m_d;
			bool cond2 = r + m_u > s;
			if (!cond1 && !cond2)
				m.push_back(d);
			else if (cond1 && !cond2) {
				m.push_back(d);
				break;
			}
			else if (!cond1 && cond2) {
				m.push_back(d + 1);
				break;
			}
			else {
				if (r << 1 < s)
					m.push_back(d);
				else if (r << 1 > s)
					m.push_back(d + 1);
				else
					m.push_back(((d & 1) == 0) ? d : d + 1);
				break;
			}
		}
		m.push_back('e');
		m += Integer(k).ToString10();
		return;
	}

	exp_type _approxmate_k(const vector<save_type>& num, const exp_type e) {
		exp_type len = 0;
		auto h = num.back();
		while (h != 0) {
			++len;
			h >>= 1;
		}
		len += (e + num.size() - 1) * BIT_NUMBER;
		len = (exp_type)(len / 3.3219280948873623478703194294894); // len / log_2(10)
		return len;
	}

	::std::string Real::ToString10() const
	{
		std::string res;
		if (_numvec.back() == 0)
			return std::string("0.0");
		res = (_signal > 0) ? "0." : "-0.";
		auto k = _approxmate_k(_numvec, _exp);
		_to_radix_10(_number, _exp, res, k);
		return res;
	}

	void Real::RealParseF(const char** it, UInteger& f, exp_type& e) {
		::std::string f_dec;
		const char* p = *it;
		e = 0;
		auto isdigit = [](char c) {return c >= '0'&&c <= '9';};
		if (!isdigit(*p))
			throw std::runtime_error("Invalid exponiential number.");
		while (isdigit(*p)) {
			f = f * (unsigned)10 + (unsigned)(*p - '0');
			++p;
		}
		if (*p == '.')
		{
			++p;
			while (isdigit(*p)) {
				f = f * (unsigned)10 + (unsigned)(*p - '0');
				++p;
				--e;
			}
		}
		*it = p;
	}

	void Real::RealParseExp(const char** it, exp_type& e) {
		const char* p = *it;
		exp_type exp = 0;
		auto isdigit = [](char c) {return c >= '0'&&c <= '9';};
		bool isneg = false;
		if (*p == 'e' || *p == 'E')
		{
			++p;
			if (*p == '-')
				isneg = true;
			if (!isdigit(*p))
				throw std::runtime_error("Invalid exponiential number.");
			while (isdigit(*p)) {
				exp = exp * 10 + *p - '0';
				++p;
			}
			if (isneg)
				exp = -exp;
		}
		*it = p;
		e += exp;
	}

	// Reference: 
	// Clinger W D. How to read floating point numbers accurately[J].
	// programming language design and implementation, 1990, 25(6): 92-101.
	inline void AlgorithmM(const UInteger& f, const exp_type e, size_t prec, UInteger& m, exp_type& k) {
		UInteger u, v;
		k = 0;
		if (e < 0) {
			u = f;
			v = UInteger((unsigned)10).Power(-e);
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
		//assert(x.size() == prec);

		if ((r << 1) < v) // err < 1/2 * precision
			m = x;
		else if ((r << 1) > v) // err > 1/2 * precision
			m = x + (unsigned)1;
		else if (IsEven(x)) // err == 1/2 * precision
			m = x;
		else
			m = x + (unsigned)1;
	}

	int Real::Parse(const char* str)
	{
		const char* it = str;
		UInteger f;
		exp_type e;
		int res = Number_Parse_OK;
		auto prec = _number.size();
		
		try {
			detail::NumberParseSignal(it, _signal);
			RealParseF(&it, f, e);
			RealParseExp(&it, e);
			if (*it != '\0')
				return Number_Parse_Failed;
			AlgorithmM(f, e, prec, _number, _exp);
			RoundTo(prec);
		}
		catch (std::exception) {
			res = Number_Parse_Failed;
		}
		return res;
	}

	void Real::RoundTo(size_t precision)
	{
		int diff = _number.size() - precision;
		if (diff == 0)
			return;
		else if (diff > 0) {
			// judge round
			bool round_up = false;
			if (_numvec[diff - 1] > (MODULE >> 1))
				round_up = true;
			else if (_numvec[diff - 1] == (MODULE >> 1)) {
				for (auto it = _numvec.crbegin() + precision + 1;
					it != _numvec.rend();++it) {
					if (*it > 0) {
						round_up = true;
						break;
					}
				}
			}

			_numvec.erase(_numvec.cbegin(), _numvec.cbegin() + diff);
			if (round_up) {
				_number = _number + (unsigned)1;
				if (_numvec.size() > precision)
					_numvec.erase(_numvec.begin());
			}
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
		RoundTo(precision);
	}

	size_t Real::GetPrecision() const
	{
		return (_number.size() - 1) * BIT_NUMBER;
	}

	size_t Real::size() const
	{
		return _number.size();
	}

}
