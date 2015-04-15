#ifndef INCLUDED_MQF_BONDS
#define INCLUDED_MQF_BONDS
#include "cashflow.h"

namespace mqf {

	template<int N,typename Date = Gregorian::Date>
	struct CouponBond {
		Cashflow<Date> principal;
		Cashflow<Date> coupons[N];
	};

}

#endif
