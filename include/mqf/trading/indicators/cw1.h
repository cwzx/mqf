#ifndef INCLUDED_MQF_TRADING_INDICATORS_CW1
#define INCLUDED_MQF_TRADING_INDICATORS_CW1
#include "../../time_series/wma.h"
#include "../../distributions/normal.h"
#include "../return.h"
#include "../../functions.h"

namespace mqf {
namespace Indicators {

	struct CW1 {
		struct Value {
			double sharpe, kelly;
			Value( double s, double k ) : sharpe(s), kelly(k) {}
		};

		int period;

		explicit CW1( int period = 100 ) : period(period) {}

		template<typename It>
		Value compute( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);

			if( count < period )
				return Value( 0.0, 0.0 );
			auto pb = std::prev(p2,period);
			auto logRet = computeLogReturns( pb, p2 );
			auto N = logRet.size();

			double t_avg = -0.5 * (N+1);
			double t2_avg = (1.0/6.0) * (N+1)*(2*N+1);
			double r_avg = meanLogReturn( pb, p2 );
			double tr_avg = -r_avg * (N+1) + WMA::weightedSum( logRet.begin(), logRet.end() ) / N;

			double mu0dt = (r_avg*(t2_avg + 0.5*t_avg) - tr_avg*(t_avg + 0.5)) / (t2_avg - t_avg*t_avg);
			double a1dt = ( tr_avg - t_avg*r_avg ) / (t2_avg - t_avg*t_avg);
			
			double b0dt = 0.0;
			for(size_t k=0;k<N;++k) {
				b0dt += square( logRet[k] - ( mu0dt + 0.5 * a1dt * (1.0 - 2.0*(N-k)) ) );
			}
			b0dt /= N;

			Distributions::Normal dist( mu0dt + 0.5 * a1dt, b0dt );

			auto sharpe = dist.mu / std::sqrt( dist.sigma2 );
			auto kelly = dist.mu / ( dist.mu * dist.mu + dist.sigma2 );

			return Value( sharpe, kelly );
		}

	};
	

}
}

#endif
