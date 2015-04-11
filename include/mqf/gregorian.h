#ifndef MQF_GREGORIAN
#define MQF_GREGORIAN
#include "chrono.h"

namespace mqf {

	struct Gregorian {

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

		static int daysInMonth( Month m, Year y ) {
			if( isLeapYear(y) )
				return daysPerMonthLeap[m-1];
			return daysPerMonthCommon[m-1];
		}

		static constexpr bool isLeapYear( Year year ) {
			return ( year % 4 == 0 )
			    && ( !( year % 100 == 0 ) || ( year % 400 == 0 ) );
		}

	};

	template<typename> Date;

	template<>
	struct Date<Gregorian> {
		using Day   = Gregorian::Day;
		using Month = Gregorian::Month;
		using Year  = Gregorian::Year;

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

		Date<Gregorian>& operator++() {
			int daysInMonth = Gregorian::daysInMonth( month, year );
			++day;
			if( day > daysInMonth ) {
				month = Month( int(month) + 1 );
				day = 1;
				if( month > Gregorian::monthsPerYear ) {
					++year;
					month = Month(1);
				}
			}
			return *this;
		}

		Date<Gregorian> operator++(int) {
			auto old = *this;
			++(*this);
			return old;
		}

		Date<Gregorian>& operator--() {
			--day;
			if( day < 1 ) {
				month = Month( int(month) - 1 );
				if( month < 1 ) {
					--year;
					month = Month(Gregorian::monthsPerYear);
				}
				day = Gregorian::daysInMonth( month, year );
			}
			return *this;
		}

		Date<Gregorian> operator--(int) {
			auto old = *this;
			--(*this);
			return old;
		}

		bool operator==( const Date<Gregorian>& rhs ) const {
			return ( day == rhs.day ) && ( month == rhs.month ) && ( year == rhs.year );
		}

		bool operator!=( const Date<Gregorian>& rhs ) const {
			return !( *this == rhs );
		}
		
		bool operator<( const Date<Gregorian>& rhs ) const {
			return ( year <  rhs.year )
			    || ( year == rhs.year && month <  rhs.month )
			    || ( year == rhs.year && month == rhs.month  && day < rhs.day );
		}

		bool operator<=( const Date<Gregorian>& rhs ) const {
			return ( *this < rhs ) || ( *this == rhs );
		}

		bool operator>( const Date<Gregorian>& rhs ) const {
			return !( year <= rhs.year );
		}

		bool operator>=( const Date<Gregorian>& rhs ) const {
			return !( year < rhs.year );
		}

	};

	inline Time Duration( Date<Gregorian> d1, Date<Gregorian> d2 ) {
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

#endif
