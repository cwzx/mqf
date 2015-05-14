#ifndef INCLUDED_MQF_TRADING_STRAT_MOVING_AVERAGE
#define INCLUDED_MQF_TRADING_STRAT_MOVING_AVERAGE
#include "../action.h"
#include "../../time_series/wma.h"

namespace mqf {

	struct MAStrategy {
		int shortPeriod, longPeriod;

		explicit MAStrategy( int shortPeriod = 20,
		                     int longPeriod = 42 ) :
			shortPeriod(shortPeriod),
			longPeriod(longPeriod)
		{}

		template<typename It>
		Action compute( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);
			if( count < std::max(shortPeriod,longPeriod) )
				return Action( Action::Hold );
		
			auto shortMA = WMA(shortPeriod).back(p1,p2);
			auto longMA = WMA(longPeriod).back(p1,p2);

			if( shortMA > longMA ) {
				return Action( Action::Buy );
			}
			if( shortMA < longMA ) {
				return Action( Action::Sell );
			}
			return Action( Action::Hold );
		}

	};

}

#endif
