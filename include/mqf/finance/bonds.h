#ifndef INCLUDED_MQF_BONDS
#define INCLUDED_MQF_BONDS
#include "cashflow.h"
#include <cstdint>

namespace mqf {

	template<typename Calendar>
	struct ZeroCouponBond {
		Cashflow<Calendar> principal;

		ZeroCouponBond() = default;
		explicit ZeroCouponBond( const Cashflow<Calendar>& principal ) : principal(principal) {}
	};

	template<typename Calendar,int N>
	struct CouponBond {
		Cashflow<Calendar> principal;
		Cashflow<Calendar> coupons[N];
	};

	template<typename Calendar,int N>
	struct FixedCouponBond {
		double principal;
		Date<Calendar> maturity;
		double rate;
		double couponInterval;
	};

	template<typename Calendar,int N>
	struct FloatingRateNote {
		Date<Calendar> initialDate;
		Date<Calendar> couponDates[N];
		double principal;
		Date<Calendar> maturity;

	};



	template<typename Calendar,Compounding c>
	double Price( const ZeroCouponBond<Calendar>& zcb, const InterestRate<c>& interest, const Date<Calendar>& date ) {
		return PresentValue( zcb.principal, interest, date );
	}


}

#endif
