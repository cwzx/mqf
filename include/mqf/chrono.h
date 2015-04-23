#ifndef INCLUDED_MQF_CHRONO
#define INCLUDED_MQF_CHRONO
#include <cstdint>
#include <ostream>
#include <chrono>

namespace mqf {

	struct FormattedTime {
		int64_t hour, minute, second, milli, micro, nano;

		explicit FormattedTime( int64_t hour   = 0,
		                        int64_t minute = 0,
		                        int64_t second = 0,
		                        int64_t milli  = 0,
		                        int64_t micro  = 0,
		                        int64_t nano   = 0 ) :
			hour(hour),
			minute(minute),
			second(second), 
			milli(milli),
			micro(micro),
			nano(nano)
		{}
	};

	std::ostream& operator<<( std::ostream& os, const FormattedTime& f ) {
		return os << f.hour << ":"
		          << f.minute << ":"
		          << f.second << ":"
		          << f.milli << ":"
		          << f.micro << ":"
		          << f.nano;
	}

	struct Time {

		int64_t nanoseconds;

		static constexpr int64_t hourPerDay      = 24;
		static constexpr int64_t minutePerHour   = 60;
		static constexpr int64_t secondPerMinute = 60;
		static constexpr int64_t milliPerSecond  = 1000;
		static constexpr int64_t microPerMilli   = 1000;
		static constexpr int64_t nanoPerMicro    = 1000;

		static constexpr int64_t minutePerDay    = minutePerHour * hourPerDay;

		static constexpr int64_t secondPerHour   = secondPerMinute * minutePerHour;
		static constexpr int64_t secondPerDay    = secondPerHour * hourPerDay;

		static constexpr int64_t milliPerMinute  = milliPerSecond * secondPerMinute;
		static constexpr int64_t milliPerHour    = milliPerMinute * minutePerHour;
		static constexpr int64_t milliPerDay     = milliPerHour * hourPerDay;

		static constexpr int64_t microPerSecond  = microPerMilli * milliPerSecond;
		static constexpr int64_t microPerMinute  = microPerSecond * secondPerMinute;
		static constexpr int64_t microPerHour    = microPerMinute * minutePerHour;
		static constexpr int64_t microPerDay     = microPerHour * hourPerDay;

		static constexpr int64_t nanoPerMilli    = nanoPerMicro * microPerMilli;
		static constexpr int64_t nanoPerSecond   = nanoPerMilli * milliPerSecond;
		static constexpr int64_t nanoPerMinute   = nanoPerSecond * secondPerMinute;
		static constexpr int64_t nanoPerHour     = nanoPerMinute * minutePerHour;
		static constexpr int64_t nanoPerDay      = nanoPerHour * hourPerDay;

		explicit Time( int64_t t = 0 ) : nanoseconds(t) {}

		explicit Time( const FormattedTime& f  ) {
			nanoseconds = f.hour   * nanoPerHour
			            + f.minute * nanoPerMinute
			            + f.second * nanoPerSecond
			            + f.milli  * nanoPerMilli
			            + f.micro  * nanoPerMicro
			            + f.nano;
		}

		static Time now() {
			using namespace std::chrono;
			int64_t t = duration_cast<std::chrono::nanoseconds>( system_clock::now().time_since_epoch() ).count();
			t -= nanoPerDay * (t / nanoPerDay);
			return Time(t);
		}
		
		FormattedTime formatted() const {
			FormattedTime f;
			
			int64_t remain = nanoseconds;
			
			f.hour = remain / nanoPerHour;
			remain -= f.hour * nanoPerHour;
			
			f.minute = remain / nanoPerMinute;
			remain -= f.minute * nanoPerMinute;

			f.second = remain / nanoPerSecond;
			remain -= f.second * nanoPerSecond;

			f.milli = remain / nanoPerMilli;
			remain -= f.milli * nanoPerMilli;

			f.micro = remain / nanoPerMicro;
			remain -= f.micro * nanoPerMicro;

			f.nano = remain;

			return f;
		}

		double seconds() const {
			return double(nanoseconds) / double(nanoPerSecond);
		}

		double days() const {
			return double(nanoseconds) / double(nanoPerDay);
		}

	};

	

}

#endif
