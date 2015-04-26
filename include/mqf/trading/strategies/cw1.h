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

		explicit CW1( double sellLimit = 0.05,
		              double buyLimit = 0.1,
					  int period = 40 ) :
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
			auto N = logRet.size();

			double t_avg = -0.5 * (N+1);
			double t2_avg = 1.0/6.0 * (N+1)*(2*N+1);
			double r_avg = sampleMean(logRet.begin(),logRet.end());
			double tr_avg = -r_avg * (N+1) + WeightedMovingAverage::weightedSum( logRet.begin(), logRet.end() ) / N;

			double mu0dt = (r_avg*(t2_avg + 0.5*t_avg) - tr_avg*(t_avg + 0.5)) / (t2_avg - t_avg*t_avg);
			double a1dt = ( tr_avg - t_avg*r_avg ) / (t2_avg - t_avg*t_avg);
			
			double b0dt = 0.0;
			for(size_t k=0;k<N;++k) {
				double d = logRet[k] - ( mu0dt + 0.5 * a1dt * (1.0 - 2.0*(N-k)) );
				b0dt += d*d;
			}
			b0dt /= N;

			Distributions::Normal dist( mu0dt + 0.5 * a1dt, b0dt );

			double h = dist.mu / std::sqrt( dist.sigma2 );
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
