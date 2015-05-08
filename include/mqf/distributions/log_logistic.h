#ifndef INCLUDED_MQF_DISTRIBUTIONS_LOG_LOGISTIC
#define INCLUDED_MQF_DISTRIBUTIONS_LOG_LOGISTIC
#include <cassert>
#include "logistic.h"

namespace mqf {
namespace Distributions {

	struct LogLogistic : Density<> {
		double alpha, beta;
				
		explicit LogLogistic( double alpha = 1.0, double beta = 1.0 ) : alpha(alpha), beta(beta) {}

		double mean() const {
			assert(beta > 1.0);
			auto c = Pi / beta;
			return ( alpha * c ) / std::sin(c);
		}

		double median() const {
			return alpha;
		}

		double mode() const {
			if(beta <= 1.0)
				return 0.0;
			return alpha * std::pow( (beta - 1.0) / (beta + 1.0), 1.0 / beta );
		}

		double variance() const {
			
		}

		double operator()( double x ) const {
			auto y = x / alpha;
			auto d = 1.0 + std::pow(y,beta);
			return beta * std::pow(y,beta-1.0) / (alpha * d * d);
		}

		double cumulative( double x ) const {
			return 1.0 / ( 1.0 + std::pow(x/alpha,-beta) );
		}

	};

	RV<LogLogistic> exp( const RV<Logistic>& x ) {
		return MakeRV( LogLogistic( std::exp(x.dist.mu), 1.0 / x.dist.s ) );
	}

	RV<Logistic> log( const RV<LogLogistic>& x ) {
		return MakeRV( Logistic( std::log(x.dist.alpha), 1.0 / x.dist.beta ) );
	}

}
}

#endif
