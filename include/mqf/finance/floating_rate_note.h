#ifndef INCLUDED_MQF_FLOATING_RATE_NOTE
#define INCLUDED_MQF_FLOATING_RATE_NOTE
#include "../gregorian.h"

namespace mqf {

	template<int N,typename Date = Gregorian::Date>
	struct FloatingRateNote {
		Date initialDate;
		Date couponDates[N];
		double principal;
		Date maturity;
	};

	template<int N,typename Date = Gregorian::Date>
	using FRN = FloatingRateNote<N,Date>;

}

#endif
