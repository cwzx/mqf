#ifndef INCLUDED_MQF_CHRONO
#define INCLUDED_MQF_CHRONO
#include <cstdint>
#include <ostream>
#include <chrono>
#include <iomanip>

namespace mqf {

	struct FormattedTime {
		uint16_t hour, minute, second, milli, micro, nano;

		explicit FormattedTime( uint16_t hour   = 0,
		                        uint16_t minute = 0,
		                        uint16_t second = 0,
		                        uint16_t milli  = 0,
		                        uint16_t micro  = 0,
		                        uint16_t nano   = 0 ) :
			hour(hour),
			minute(minute),
			second(second), 
			milli(milli),
			micro(micro),
			nano(nano)
		{}
	};

	std::ostream& operator<<( std::ostream& os, const FormattedTime& f ) {
		using namespace std;
		return os << setfill('0')
		          << setw(2) << f.hour   << ":"
		          << setw(2) << f.minute << ":"
		          << setw(2) << f.second << "."
		          << setw(3) << f.milli  << "\'"
		          << setw(3) << f.micro  << "\'"
		          << setw(3) << f.nano;
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
			
			f.hour = uint16_t(remain / nanoPerHour);
			remain -= f.hour * nanoPerHour;
			
			f.minute = uint16_t(remain / nanoPerMinute);
			remain -= f.minute * nanoPerMinute;

			f.second = uint16_t(remain / nanoPerSecond);
			remain -= f.second * nanoPerSecond;

			f.milli = uint16_t(remain / nanoPerMilli);
			remain -= f.milli * nanoPerMilli;

			f.micro = uint16_t(remain / nanoPerMicro);
			remain -= f.micro * nanoPerMicro;

			f.nano = uint16_t(remain);

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
