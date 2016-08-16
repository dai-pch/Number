#include "Integer.h"

Integer::Integer(vector<save_type> Number, char Signal = 1, uint64 Exponential = 0) :_number(Number), _signal(Signal), _exponential(Exponential) {}

Integer::Integer(const Integer& src) : _number(src._number), _signal(src._signal), _exponential(src._exponential) {}

Integer::Integer(const int src) : _number(src>0 ? src : -src), _signal(src>0 ? 1 : -1), _exponential(0) {}

Integer::Integer(const unsigned int src) : _number(src), _signal(1), _exponential(0) {}

int Integer::setSignal(const int& signal)
{
	return _signal = signal >= 0 ? 1 : -1;
}

/*int Integer::negative()
{
	return _signal = -_signal;
}*/

Integer Integer::operator- () const
{
	return Integer(_number, -_signal, _exponential);
}

Integer Integer::operator+ (const Integer& Obj2) const
{
	uint64 size1 = this->_number.size(), size2 = Obj2._number.size();
	uint64 exp1 = this->_exponential, exp2 = Obj2._exponential;
	
	Integer result;
	return result;
}
