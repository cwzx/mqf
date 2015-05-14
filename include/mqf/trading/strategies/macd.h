#ifndef INCLUDED_MQF_TRADING_STRAT_MACD
#define INCLUDED_MQF_TRADING_STRAT_MACD
#include "../action.h"
#include "../indicators/macd.h"

namespace mqf {
namespace Strategies {

	struct MACD {

		Indicators::MACD macd;

		explicit MACD( int shortPeriod = 12,
		               int longPeriod = 26,
		               int signalPeriod = 9 ) :
			macd(shortPeriod,longPeriod,signalPeriod)
		{}

		template<typename It>
		Action compute( It p1, It p2 ) const {
			auto r = macd.compute(p1,p2);

			if( r > 0.0 ) {
				return Action( Action::Buy );
			}
			if( r < 0.0 ) {
				return Action( Action::Sell );
			}
			return Action( Action::Hold );
		}

	};

}
}

#endif
