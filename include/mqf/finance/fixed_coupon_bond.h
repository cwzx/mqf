#ifndef INCLUDED_MQF_FIXED_COUPON_BOND
#define INCLUDED_MQF_FIXED_COUPON_BOND
#include <cstdint>
#include "../gregorian.h"

namespace mqf {

	template<typename Date = Gregorian::Date>
	struct FixedCouponBond {
		double principal;
		double rate;
		double couponInterval;
		Date maturity;
		int N;

		Date getDate( int i ) const {
			return maturity - delta * (N - i);
		}
	};

	template<typename Date,typename YieldCurve>
	double MacaulayDuration( const FixedCouponBond<Date>& bond, const YieldCurve& yieldCurve, const Date& now ) {
		double s1 = 0.0;
		double s2 = 0.0;
		int N = bond.N;
		for(uint32_t i=0;i<N;++i) {
			auto p = Price( ZCB<Calendar>( bond.getDate(i) ), yieldCurve, now );
			s1 += p * i;
			s2 += p;
		}
		return Duration( now, bond.maturity ) - bond.couponInterval * ( N - s1 / s2 );
	}

}

#endif
