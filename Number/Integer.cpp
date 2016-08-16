#include "Integer.h"

Integer::Integer(vector<save_type> Number, char Signal, uint64 Exponential) :_number(Number), _signal(Signal), _exponential(Exponential) {}

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

Integer Integer::operator-() const
{
	return Integer(_number, -_signal, _exponential);
}

Integer Integer::operator+(const Integer& Obj2) const
{
	//若两数符号不同，则实际为减法
	if (this->_signal != Obj2._signal)
		return (*this - (-Obj2));

	const Integer *p1, *p2;
	this->_exponential > Obj2._exponential ? (p2 = this, p1 = &Obj2) : (p2 = &Obj2, p1 = this);

	//uint64 size1 = p1->_number.size(), size2 = p2->_number.size();
	uint64 expo1 = p1->_exponential, expo2 = p2->_exponential;
	uint64 sub1 = expo1 + p1->_number.size(), sub2 = expo2 + p2->_number.size();

	if (sub1 <= expo2)
	{
		vector<save_type> result(sub2 - expo1, 0);
		copy(p1->_number.begin(), p1->_number.end(), result.begin());
		copy(p2->_number.rbegin(), p2->_number.rend(), result.rbegin());
		return Integer(result, _signal, p1->_exponential);
	}
	else if (sub2 <= sub1)
	{
		vector<save_type> result(sub1 - expo1);
		vector<save_type>::const_iterator it1 = p1->_number.begin(), it2 = p2->_number.begin();
		vector<save_type>::iterator it_result = result.begin();
		for (int ii = 0;ii < expo2 - expo1;ii++, it1++, it_result++)
			*it_result = *it1;

		calc_type temp = 0;
		unsigned char c = 0;
		for (int ii = 0;ii < sub2 - expo2;ii++, it1++, it2++, it_result++)
		{
			temp = *it1 + *it2 + c;
			*it_result = (save_type)(temp % MODEL);
			c = (char)(temp / MODEL);
		}

		for (int ii = 0;ii < sub1 - sub2;ii++, it1++, it_result++)
		{
			temp = *it1 + c;
			*it_result = (save_type)(temp % MODEL);
			c = (char)(temp / MODEL);
		}
		if (c != 0)
			result.push_back(c);
	}
	else
	{
		vector<save_type> result(sub2 - expo1);
		vector<save_type>::const_iterator it1 = p1->_number.begin(), it2 = p2->_number.begin();
		vector<save_type>::iterator it_result = result.begin();
		for (int ii = 0;ii < expo2 - expo1;ii++, it1++, it_result++)
			*it_result = *it1;

		calc_type temp = 0;
		unsigned char c = 0;
		for (int ii = 0;ii < sub2 - expo2;ii++, it1++, it2++, it_result++)
		{
			temp = *it1 + *it2 + c;
			*it_result = (save_type)(temp % MODEL);
			c = (char)(temp / MODEL);
		}

		for (int ii = 0;ii < sub1 - sub2;ii++, it1++, it_result++)
		{
			temp = *it1 + c;
			*it_result = (save_type)(temp % MODEL);
			c = (char)(temp / MODEL);
		}
		if (c != 0)
			result.push_back(c);
	}

	Integer result;
	return result;
}

Integer Integer::operator-(const Integer &) const
{
	return Integer();
}
