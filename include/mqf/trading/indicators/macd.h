#ifndef INCLUDED_MQF_TRADING_INDICATORS_MACD
#define INCLUDED_MQF_TRADING_INDICATORS_MACD
#include "../../time_series/ema.h"
#include "../../time_series/wma.h"
#include "../return.h"
#include <algorithm>
#include <functional>

namespace mqf {
namespace Indicators {

	struct MACD {
		using Value = double;

		EMA ema_short, ema_long, ema_signal;

		explicit MACD( int shortPeriod = 12, int longPeriod = 26, int signalPeriod = 9 ) :
			ema_short(shortPeriod),
			ema_long(longPeriod),
			ema_signal(signalPeriod)
		{}

		template<typename It>
		std::vector<Value> compute( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);
			
			// compute the short/long EMAs
			auto macd = ema_short.compute(p1,p2);
			auto val_long = ema_long.compute(p1,p2);

			// subtract the long
			std::transform( macd.begin(), macd.end(), val_long.begin(), macd.begin(), std::minus<>() );
			
			// compute the signal
			auto val_signal = ema_signal.compute( macd.begin(), macd.end() );
			
			// subtract the signal
			std::transform( macd.begin(), macd.end(), val_signal.begin(), macd.begin(), std::minus<>() );

			return macd;
		}

	};
	

}
}

#endif
