#ifndef MQF_DAY_COUNT
#define MQF_DAY_COUNT
#include "gregorian.h"
#include <algorithm>

namespace mqf {

	enum DayCountConvention {
		BondBasis_30360,
		E_30360,
		Actual
	};

	template<DayCountConvention>
	struct DayCount {};

	template<int daysPerMonth,int daysPerYear>
	inline Time DurationFixed( const Date<>& d1, const Date<>& d2 ) {
		return Time( (d2.day - d1.day) + (d2.month - d1.month) * daysPerMonth + (d2.year - d1.year) * daysPerYear ) / daysPerYear;
	}

	inline Time Duration( Date<> d1, Date<> d2, DayCount<BondBasis_30360> ) {
		d1.day = std::min( d1.day, 30 );
		if( d1.day == 30 )
			d2.day = std::min( d2.day, 30 );
		return DurationFixed<30,360>( d1, d2 );
	}

	inline Time Duration( Date<> d1, Date<> d2, DayCount<E_30360> ) {
		d1.day = std::min( d1.day, 30 );
		d2.day = std::min( d2.day, 30 );
		return DurationFixed<30,360>( d1, d2 );
	}

	inline Time Duration( Date<> d1, Date<> d2, DayCount<Actual> ) {
		return Duration( d1, d2 );
	}

	template<DayCountConvention DCC = Actual>
	inline Time Duration( const Date<>& d1, const Date<>& d2 ) {
		return Duration( d1, d2, DayCount<DCC>() );
	}

}



#endif
