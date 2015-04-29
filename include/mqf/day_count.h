#ifndef INCLUDED_MQF_DAY_COUNT
#define INCLUDED_MQF_DAY_COUNT
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
	inline double DurationFixed( const Gregorian::Date& d1, const Gregorian::Date& d2 ) {
		return double( (d2.day - d1.day) + (d2.month - d1.month) * daysPerMonth + (d2.year - d1.year) * daysPerYear ) / daysPerYear;
	}

	inline double Duration( Gregorian::Date d1, Gregorian::Date d2, DayCount<BondBasis_30360> ) {
		d1.day = std::min( d1.day, 30 );
		if( d1.day == 30 )
			d2.day = std::min( d2.day, 30 );
		return DurationFixed<30,360>( d1, d2 );
	}

	inline double Duration( Gregorian::Date d1, Gregorian::Date d2, DayCount<E_30360> ) {
		d1.day = std::min( d1.day, 30 );
		d2.day = std::min( d2.day, 30 );
		return DurationFixed<30,360>( d1, d2 );
	}

	inline double Duration( Gregorian::Date d1, Gregorian::Date d2, DayCount<Actual> ) {
		return yearFraction( d1, d2 );
	}

	template<DayCountConvention DCC = Actual>
	inline double Duration( const Gregorian::Date& d1, const Gregorian::Date& d2 ) {
		return Duration( d1, d2, DayCount<DCC>() );
	}

}



#endif
