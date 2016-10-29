#include "Real.h"

namespace Number {
	Real::Real(Real &Obj) :_number(Obj._number),
		_exp(Obj._exp), _tolerance(Obj._tolerance),
		_tolexp(Obj._tolexp) {}

	Real::Real(Integer &Obj): _number(Obj) {}

	int Real::Compare(const Real &) const
	{
		return 0;
	}


}
