#ifndef INCLUDED_MQF_TRADING_STRAT_CW1
#define INCLUDED_MQF_TRADING_STRAT_CW2
#include "../action.h"
#include "../../time_series/moving_average.h"
#include "../../distributions/normal.h"

namespace mqf {

	template<typename It>
	std::vector<double> logReturns( It p1, It p2 ) {
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

	struct CW1 {
		double sellLimit, buyLimit;
		int period;

		explicit CW1( double sellLimit = 0.0,
		              double buyLimit = 0.1,
					  int period = 20 ) :
			sellLimit(sellLimit),
			buyLimit(buyLimit),
			period(period)
		{}
		
		template<typename It>
		Action compute( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);
			
			if( count < period )
				return Action( Action::Hold, 0 );

			auto logRet = logReturns(std::next(p2,-period),p2);
			Distributions::Normal N;
			N.mu = sampleMean(logRet.begin(),logRet.end());
			N.sigma2 = biasedSampleVariance(logRet.begin(),logRet.end());

			double h = N.mu / std::sqrt( N.sigma2 );
			if( h > buyLimit ) {
				return Action( Action::Buy, h );
			}
			if( h < sellLimit ) {
				return Action( Action::Sell, h );
			}
			return Action( Action::Hold, h );
		}

	};

}

#endif
