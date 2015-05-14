#ifndef INCLUDED_MQF_TRADING_INDICATORS_MACD
#define INCLUDED_MQF_TRADING_INDICATORS_MACD
#include "../../time_series/ema.h"
#include "../../time_series/wma.h"
#include "../return.h"

namespace mqf {
namespace Indicators {

	struct MACD {
		using Value = double;

		WMA ema_short, ema_long, ema_signal;

		explicit MACD( int shortPeriod = 12, int longPeriod = 26, int signalPeriod = 9 ) :
			ema_short(shortPeriod),
			ema_long(longPeriod),
			ema_signal(signalPeriod)
		{}

		template<typename It>
		Value compute( It p1, It p2 ) const {
			auto val_short  = ema_short.back(p1,p2);
			auto val_long   = ema_long.back(p1,p2);
			auto val_signal = ema_signal.back(p1,p2);

			auto macd = val_short - val_long;

			return macd - val_signal;
		}

	};
	

}
}

#endif
