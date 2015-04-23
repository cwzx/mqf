#ifndef INCLUDED_MQF_TRADING_STRAT_STRIPES
#define INCLUDED_MQF_TRADING_STRAT_STRIPES
#include "../action.h"

namespace mqf {

	template<typename It>
	std::vector<double> logPrices( It p1, It p2 ) {
		auto count = std::distance(p1,p2);
		std::vector<double> results;
		results.reserve(count);
		for(;p1!=p2;++p1) {
			results.push_back( std::log( *p1 ) );
		}
		return results;
	}

	struct Stripes {
		double sell_size, hold_size, buy_size;

		explicit Stripes( double sell_size = 0.001,
		                  double hold_size = 0.005,
		                  double buy_size  = 0.001 ) :
			sell_size(sell_size),
			hold_size(hold_size),
			buy_size(buy_size)
		{}
		
		template<typename It>
		Action compute( It p1, It p2 ) const {
			if( std::distance(p1,p2) < 2 )
				return Action();

			double current = *--p2;
			double previous = *--p2;
			bool up = previous < current;

			while(true) {
				if( current < sell_size ) {
					if( !up )
						return Action( Action::Sell );
					break;
				}
				current -= sell_size;
				if( current < buy_size ) {
					if( up )
						return Action( Action::Buy );
					break;
				}
				current -= buy_size;
				if( current < hold_size ) {
					break;
				}
				current -= hold_size;
			}
			return Action( Action::Hold );
		}

	};

}

#endif
