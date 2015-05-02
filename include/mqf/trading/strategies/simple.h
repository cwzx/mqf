#ifndef INCLUDED_MQF_TRADING_STRAT_SIMPLE
#define INCLUDED_MQF_TRADING_STRAT_SIMPLE
#include "../action.h"
#include "../../time_series/moving_average.h"

namespace mqf {

	struct SimpleStrategy {
		int period = 5;

		template<typename It>
		Action compute( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);
			if( count < period )
				return Action( Action::Hold );
		
			auto ma = WeightedMovingAverage(period).compute( std::prev(p2,period), p2 );

			auto current = *--p2;
			auto avg = ma.back();

			if( current > avg )
				return Action( Action::Buy );

			if( current < avg )
				return Action( Action::Sell );

			return Action( Action::Hold );
		}

	};

}

#endif
