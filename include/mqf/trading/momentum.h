#ifndef INCLUDED_MQF_TRADING_MOMENTUM
#define INCLUDED_MQF_TRADING_MOMENTUM
#include <mqf/stats/descriptive.h>

namespace mqf {

	struct Strategy1 {
		double sellLimit, buyLimit;

		explicit Strategy1( double sellLimit = 0.0,
							double buyLimit = 1.0 ) :
			sellLimit(sellLimit),
			buyLimit(buyLimit)
		{}

		template<typename It>
		Action compute( It p1, It p2 ) const {
			double mu, sigma2;
			double h = (mu - 0.5 * sigma2) / sigma2;
			if( h > buyLimit ) {
				return Action( Action::Buy );
			}
			if( h < sellLimit ) {
				return Action( Action::Sell );
			}
			return Action( Action::Hold );
		}

	};

	struct MAStrategy {
		int shortPeriod, longPeriod;

		explicit MAStrategy( int shortPeriod = 50,
							 int longPeriod = 200 ) :
			shortPeriod(shortPeriod),
			longPeriod(longPeriod)
		{}

		template<typename It>
		Action compute( It p1, It p2 ) const {
		
			auto shortMA = SimpleMovingAverage(shortPeriod).compute(p1,p2);
			auto longMA = SimpleMovingAverage(longPeriod).compute(p1,p2);
		
			if( shortMA.size() == 0 || longMA.size() == 0 )
				return Action( Action::Hold );

			if( shortMA.back() > longMA.back() ) {
				return Action( Action::Buy );
			}
			if( shortMA.back() < longMA.back() ) {
				return Action( Action::Sell );
			}
			return Action( Action::Hold );
		}

	};

	struct SimpleStrategy {

		template<typename It>
		Action compute( It p1, It p2 ) const {
			if( p1 == p2 ) return Action();
			auto current = *--p2;
			if( p1 == p2 ) return Action();
			auto previous = *--p2;
			if( current > previous )
				return Action( Action::Buy );
			if( current < previous )
				return Action( Action::Sell );
			return Action( Action::Hold );
		}

	};

}

#endif
