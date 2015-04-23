#ifndef INCLUDED_MQF_TRADING_STRAT_MOVING_AVERAGE
#define INCLUDED_MQF_TRADING_STRAT_MOVING_AVERAGE
#include "../action.h"
#include "../../time_series/moving_average.h"

namespace mqf {

	struct MAStrategy {
		int shortPeriod, longPeriod;

		explicit MAStrategy( int shortPeriod = 15,
							 int longPeriod = 30 ) :
			shortPeriod(shortPeriod),
			longPeriod(longPeriod)
		{}

		template<typename It>
		Action compute( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);
			if( count < longPeriod )
				return Action( Action::Hold );
		
			auto shortMA = WeightedMovingAverage(shortPeriod).compute(p1,p2);
			auto longMA = WeightedMovingAverage(longPeriod).compute(p1,p2);

			if( shortMA.back() > longMA.back() ) {
				return Action( Action::Buy );
			}
			if( shortMA.back() < longMA.back() ) {
				return Action( Action::Sell );
			}
			return Action( Action::Hold );
		}

	};

}

#endif
