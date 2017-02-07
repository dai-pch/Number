#include "Integer.h"
#include <algorithm>

using ::std::vector;
using ::std::string;

namespace Number {

	Integer::Integer(const vector<save_type>& Number, char Signal) 
		: _number(Number), _signal(Signal) {}

	Integer::Integer(vector<save_type>&& Number, char Signal)
		: _number(Number), _signal(Signal) {};

	Integer::Integer(const Integer& src) : _number(src._number), _signal(src._signal) {}

	Integer::Integer(const UInteger & Number, const char Signal)
		: _number(Number), _signal(Signal > 0 ? 1 : -1) {}


	int Integer::setSignal(const int& signal)
	{
		return _signal = signal >= 0 ? 1 : -1;
	}

	//��ȥ���ܵĸ�λ����
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

	int Integer::Compare(const Integer &Obj2) const
	{
		if (_number._number[0] == 0 && Obj2._number._number[0] == 0)
			return 0;
		//���Ų�ֱͬ���ж�
		else if (_signal != Obj2._signal)
			return static_cast<int>(_signal - Obj2._signal);
		else {
			auto res = _number.Compare(Obj2._number);
			return _signal > 0 ? res : -res;
		}
	}

	Integer Integer::operator<<(int num) const
	{
		UInteger result;
		result << num;
		return Integer(result, _signal);
	}

	Integer Integer::operator>>(int num) const {
		UInteger result;
		result >> num;
		return Integer(result, _signal);
	}

	// Minus
	Integer Integer::operator-() const {
		return Integer(_number, -_signal);
	}
	// Add
	Integer Integer::Add(const Integer& Obj2) const
	{
		//���������Ų�ͬ����ʵ��Ϊ����
		if (this->_signal != Obj2._signal)
			return this->Sub(-Obj2);
		return Integer(_number + Obj2._number, _signal);
	}

	Integer Integer::Sub(const Integer& Obj2) const	{
		//���������Ų�ͬ����ʵ��Ϊ�ӷ�
		if (this->_signal != Obj2._signal)
			return Integer(_number + Obj2._number, _signal);

		if (_number < Obj2._number)
			return Integer(Obj2._number.Sub(_number), -_signal);
		else
			return Integer(_number.Sub(Obj2._number), _signal);
	}

	Integer Integer::Multiply(const Integer& Obj2) const {
		//signal
		auto signal = this->_signal * Obj2._signal;
		return Integer(_number.Multiply(Obj2._number), signal);
	}

	void Devide(const Integer& Obj1, const Integer& Obj2, Integer& mod, Integer& quotient)
	{
		Devide(Obj1._number, Obj2._number, mod._number, quotient._number);
		quotient._signal = Obj1._signal * Obj2._signal;
		mod._signal = Obj1._signal;
	}

	::std::ostream & operator<<(::std::ostream & os, const Integer & Obj)
	{
		// TODO: �ڴ˴����� return ���
		return (os << Obj.ToString10());
	}

	::std::istream & operator>>(::std::istream &is, Integer &Num)
	{
		// TODO: �ڴ˴����� return ���
		::std::string str;
		is >> str;
		Num.Parse(str);
		return is;
	}

	string Integer::ToString10() const {
		string result = _number.ToString10();
		if (_signal < 0)
			result += "-";
		return result;
	}

	::std::string Integer::ToString16() const
	{
		string result = _number.ToString16();
		if (_signal < 0)
			result += "-";
		return result;
	}

	//���ַ���������ֵ
	//֧��2��8��10��16����
	//�ʷ�����(+|-|��)((0b num2 num2*)|(0o num8 num8*)|(0x num16 num16*)|(num10 num10*))
	int Integer::Parse(::std::string str)
	{
		if (str.back() != '\0')
			str.push_back('\0');
		auto it = str.cbegin();
		//�������
		detail::NumberParseSignal(it, _signal);
		str.erase(it);
		return _number.Parse(str);
	}

	size_t Integer::get_digit_number() const
	{
		return _number._number.size();
	}

	UInteger Integer::Abs() const
	{
		return _number;
	}

	Integer Integer::Power(save_type Exp) const {
		return Integer(_number.Power(Exp), (Exp & 1) == 0 ? 1 : _signal);
	}

	Integer Integer::Power(const UInteger & Exp) const {
		return Integer(_number.Power(Exp), (Exp._number[0] & 1) == 0 ? 1 : _signal);
	}

}
