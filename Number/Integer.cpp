#include "Integer.h"

Integer::Integer(vector<save_type> Number, char Signal) : _number(Number), _signal(Signal) {}

Integer::Integer(const Integer& src) : _number(src._number), _signal(src._signal) {}

Integer::Integer(const int src) : _signal(src > 0 ? 1 : -1) 
{
	_number[0] =  src > 0 ? src : -src;
}

Integer::Integer(const unsigned int src) : _signal(1) 
{
	_number[0] = src;
}

int Integer::setSignal(const int& signal)
{
	return _signal = signal >= 0 ? 1 : -1;
}

/*int Integer::negative()
{
	return _signal = -_signal;
}*/


bool Integer::operator>(const Integer& Obj2) const
{
	//两数均为零时有符号问题，单独考虑
	if (_number.back() == 0 && Obj2._number.back() == 0)
		return false;
	//符号不同直接判断
	if (_signal != Obj2._signal)
		return _signal > Obj2._signal ? true : false;
	//位数不同直接判断
	if (_number.size() != Obj2._number.size())
		return _number.size() > Obj2._number.size() ? true : false;
	//逐位比较绝对值大小
	vector<save_type>::const_reverse_iterator it1=_number.rbegin(), it2=Obj2._number.rbegin();
	while (it1 != _number.rend()-1 && *it1 != *it2)
	{
		it1++;
		it2++;
	}
	return _signal>0?*it1>*it2:*it1<*it2;
}

bool Integer::operator<(const Integer& Obj2) const
{
	//两数均为零时有符号问题，单独考虑
	if (_number.back() == 0 && Obj2._number.back() == 0)
		return false;
	//符号不同直接判断
	if (_signal != Obj2._signal)
		return _signal < Obj2._signal ? true : false;
	//位数不同直接判断
	if (_number.size() != Obj2._number.size())
		return _number.size() < Obj2._number.size() ? true : false;
	//逐位比较绝对值大小
	vector<save_type>::const_reverse_iterator it1 = _number.rbegin(), it2 = Obj2._number.rbegin();
	while (it1 != _number.rend() - 1 && *it1 != *it2)
	{
		it1++;
		it2++;
	}
	return _signal>0 ? *it1<*it2:*it1>*it2;
}

bool Integer::operator==(const Integer& Obj2) const
{
	//两数均为零时有符号问题，单独考虑
	if (_number.back() == 0 && Obj2._number.back() == 0)
		return true;
	//符号不同直接判断
	if (_signal != Obj2._signal)
		return false;
	//位数不同直接判断
	if (_number.size() != Obj2._number.size())
		return false;
	//逐位比较绝对值大小
	vector<save_type>::const_reverse_iterator it1 = _number.rbegin(), it2 = Obj2._number.rbegin();
	while (it1 != _number.rend())
	{
		if (*it1 != *it2)
			return false;
		it1++;
		it2++;
	}
	return true;
}

bool Integer::operator>=(const Integer& Obj2) const
{
	return !(*this < Obj2);
}

bool Integer::operator<=(const Integer& Obj2) const
{
	return !(*this > Obj2);
}


Integer Integer::operator-() const
{
	return Integer(_number, -_signal);
}

Integer Integer::operator+(const Integer& Obj2) const
{
	//若两数符号不同，则实际为减法
	if (this->_signal != Obj2._signal)
		return (*this - (-Obj2));

	const Integer *p1, *p2;
	this->_number.size() > Obj2._number.size() ? (p1 = &Obj2, p2 = this) : (p1 = this, p2 = &Obj2);

	uint64 size1 = p1->_number.size(), size2 = p2->_number.size();
	vector<save_type> result(size2);
	vector<save_type>::const_iterator it1 = p1->_number.begin(), it2 = p2->_number.begin();
	vector<save_type>::iterator it_result = result.begin();

	calc_type temp = 0;
	unsigned char c = 0;
	//int ii = 0;
	for (;it1 != p1->_number.end();it1++, it2++, it_result++)
	{
		temp = *it1 + *it2 + c;
		*it_result = (save_type)(temp % MODEL);
		c = (char)(temp / MODEL);
	}
	
	for (;it2 != p2->_number.end();it2++, it_result++)
	{
		temp = *it2 + c;
		*it_result = (save_type)(temp % MODEL);
		c = (char)(temp / MODEL);
	}
	
	if (c != 0)
		result.push_back(c);

	return Integer(result, _signal);
}

Integer Integer::operator-(const Integer& Obj2) const
{
	//若两数符号不同，则实际为加法
	if (this->_signal != Obj2._signal)
		return (*this + (-Obj2));

	bool gt;
	//比较两数绝对值的大小，绝对值大的赋给p1
	const Integer *p1 = nullptr, *p2 = nullptr;
	int nonEqualCounter_re = 0;
	if (_number.size() != Obj2._number.size()) //位数不同直接判断
		gt = _number.size() > Obj2._number.size() ? true : false;
	else //否则逐位比较绝对值大小
	{
		vector<save_type>::const_reverse_iterator it1 = _number.rbegin(), it2 = Obj2._number.rbegin();
		while (it1 != _number.rend() - 1 && *it1 != *it2)
		{
			it1++;
			it2++;
			nonEqualCounter_re++;
		}
		gt = *it1 > *it2 ? true : false;
	}
	gt ? (p1 = this, p2 = &Obj2) : (p1 = &Obj2, p2 = this);

	uint64 size1 = p1->_number.size(), size2 = p2->_number.size();
	vector<save_type> result(size1 - nonEqualCounter_re);
	vector<save_type>::const_iterator it1 = p1->_number.begin(), it2 = p2->_number.begin();
	vector<save_type>::iterator it_result = result.begin();
	calc_type temp = 0;
	unsigned char c = 0;
	//int ii = 0;
	for (;it1 != p1->_number.end() - nonEqualCounter_re;it1++, it2++, it_result++)
	{
		temp = *it1 - *it2 + c;
		c = (char)(temp / MODEL);
		temp += MODEL;
		*it_result = (save_type)(temp % MODEL);
	}

	for (;it2 != p2->_number.end() - nonEqualCounter_re;it2++, it_result++)
	{
		temp = *it2 + c;
		c = (char)(temp / MODEL);
		temp += MODEL;
		*it_result = (save_type)(temp % MODEL);
	}

	//除去可能的高位的零
	vector<save_type>::reverse_iterator it_result_re = result.rbegin();
	nonEqualCounter_re = 0;
	while ((it_result_re != result.rend() - 1) && *it_result_re == 0)
	{
		nonEqualCounter_re++;
		it_result_re++;
	}
	result.erase(result.end()-nonEqualCounter_re, result.end());

	return Integer(result, gt ? _signal : -_signal);
}
