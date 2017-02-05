#include "Integer.h"
#include "Real.h"
#include <algorithm>

namespace Number {

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


	int Real::Parse(const ::std::string& str)
	{
		auto it = str.begin();
		::std::string r;

		detail::NumberParseSignal(it, _signal);

		return 0;
	}

}
