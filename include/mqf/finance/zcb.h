#ifndef INCLUDED_MQF_ZCB
#define INCLUDED_MQF_ZCB
#include "cashflow.h"

namespace mqf {

	template<typename Date = Gregorian::Date>
	struct ZeroCouponBond {
		Cashflow<Date> principal;

		ZeroCouponBond() = default;
		explicit ZeroCouponBond( const Cashflow<Date>& principal ) : principal(principal) {}
		explicit ZeroCouponBond( const Date& date, double amount = 1  ) : principal(amount,date) {}
	};

	template<typename Date = Gregorian::Date>
	using ZCB = ZeroCouponBond<Date>;

	template<typename Date = Gregorian::Date>
	double MacaulayDuration( const ZCB<Date>& zcb, const Date& now ) {
		return Duration( now, zcb.principal.date );
	}

	template<typename Date,typename YieldCurve>
	double Price( const ZCB<Date>& zcb, const YieldCurve& yieldCurve, const Date& date ) {
		return yieldCurve.zcb( Duration( date, zcb.principal.date ) );
	}

}

#endif
