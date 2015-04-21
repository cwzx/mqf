#ifndef INCLUDED_MQF_TRADING_MOMENTUM
#define INCLUDED_MQF_TRADING_MOMENTUM
#include "action.h"
#include "../stats/descriptive.h"
#include "../distributions/normal.h"
#include "../time_series/moving_average.h"
#include <vector>

namespace mqf {

	struct Strategy1 {
		double sellLimit, buyLimit;

		explicit Strategy1( double sellLimit = 0.1,
							double buyLimit = 0.1 ) :
			sellLimit(sellLimit),
			buyLimit(buyLimit)
		{}

		template<typename It>
		std::vector<double> logReturns( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);
			std::vector<double> results;
			results.reserve(count-1);
			auto q = p1++;
			for(;p1!=p2;++p1) {
				results.push_back( logReturn( *q, *p1 ) );
				++q;
			}
			return results;
		}

		template<typename It>
		Action compute( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);
			int period = 20;
			
			if( count < period )
				return Action( Action::Hold );

			auto logRet = logReturns(std::next(p2,-period),p2);
			Distributions::Normal N;
			N.mu = sampleMean(logRet.begin(),logRet.end());
			N.sigma2 = sampleVariance(logRet.begin(),logRet.end());

			double h = N.mu / std::sqrt( N.sigma2 );
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

		explicit MAStrategy( int shortPeriod = 10,
							 int longPeriod = 20 ) :
			shortPeriod(shortPeriod),
			longPeriod(longPeriod)
		{}

		template<typename It>
		Action compute( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);
			if( count < longPeriod )
				return Action( Action::Hold );
		
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
