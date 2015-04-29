#ifndef INCLUDED_MQF_GREGORIAN
#define INCLUDED_MQF_GREGORIAN
#include "chrono.h"

namespace mqf {
namespace Gregorian {

	using Day = int;
	using Year = int;

	enum Month : int {
		January = 1,
		February,
		March,
		April,
		May,
		June,
		July,
		August,
		September,
		October,
		November,
		December
	};

	static constexpr int daysPerMonthCommon[12] = {
		31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};

	static constexpr int daysPerMonthLeap[12] = {
		31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
	};

	static constexpr int monthsPerYear = 12;
	static constexpr int daysPerYearCommon = 365;
	static constexpr int daysPerYearLeap = 366;
	static constexpr double daysPerYearAverage = 365.2425;

	static constexpr bool isLeapYear( Year year ) {
		return ( year % 4 == 0 )
			&& ( !( year % 100 == 0 ) || ( year % 400 == 0 ) );
	}

	static /*constexpr*/ int daysInMonth( Month m, Year y ) {
		if( isLeapYear(y) )
			return daysPerMonthLeap[int(m)-1];
		return daysPerMonthCommon[int(m)-1];
	}

	struct Date {
		Day day;
		Month month;
		Year year;

		explicit Date( Day d = 1,
		               Month m = Month(1),
		               Year y = 2000 ) :
			day(d),
			month(m),
			year(y)
		{}

		Date& operator++() {
			int maxDay = daysInMonth( month, year );
			++day;
			if( day > maxDay ) {
				month = Month( int(month) + 1 );
				day = 1;
				if( month > monthsPerYear ) {
					++year;
					month = Month(1);
				}
			}
			return *this;
		}

		Date operator++(int) {
			auto old = *this;
			++(*this);
			return old;
		}

		Date& operator--() {
			--day;
			if( day < 1 ) {
				month = Month( int(month) - 1 );
				if( month < 1 ) {
					--year;
					month = Month(monthsPerYear);
				}
				day = daysInMonth( month, year );
			}
			return *this;
		}

		Date operator--(int) {
			auto old = *this;
			--(*this);
			return old;
		}

		bool operator==( const Date& rhs ) const {
			return ( day == rhs.day ) && ( month == rhs.month ) && ( year == rhs.year );
		}

		bool operator!=( const Date& rhs ) const {
			return !( *this == rhs );
		}
		
		bool operator<( const Date& rhs ) const {
			return ( year <  rhs.year )
			    || ( year == rhs.year && month <  rhs.month )
			    || ( year == rhs.year && month == rhs.month  && day < rhs.day );
		}

		bool operator<=( const Date& rhs ) const {
			return ( *this < rhs ) || ( *this == rhs );
		}

		bool operator>( const Date& rhs ) const {
			return !( year <= rhs.year );
		}

		bool operator>=( const Date& rhs ) const {
			return !( year < rhs.year );
		}

	};

	inline int Days( Date d1, Date d2 ) {
		int count = 0;
		while( d1 < d2 ) {
			++d1;
			++count;
		}
		while( d1 > d2 ) {
			--d1;
			--count;
		}
		return count;
	}

}

	inline double yearFraction( const Gregorian::Date& d1, const Gregorian::Date& d2 ) {
		return double( Gregorian::Days(d1,d2) ) / Gregorian::daysPerYearAverage;
	}

}

#endif
