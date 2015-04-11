#ifndef INCLUDED_MQF_DISTRIBUTIONS_BETA
#define INCLUDED_MQF_DISTRIBUTIONS_BETA
#include "../distribution.h"
#include <cassert>
#include <cmath>

namespace mqf {
namespace Distributions {

	struct Beta : Density<> {
		double alpha, beta;

		Beta( double alpha = 1.0, double beta = 1.0 ) : alpha(alpha), beta(beta) {}

		double mean() const {
			return alpha / ( alpha + beta );
		}

		double mode() const {
			assert( alpha > 1 && beta > 1 );
			return ( alpha - 1.0 ) / ( alpha + beta - 2.0 );
		}

		double variance() const {
			double sum = alpha + beta;
			return ( alpha * beta ) / ( sum * sum * ( sum + 1.0 ) );
		}

		double operator()( Value x ) const {
			return std::exp( std::lgamma(alpha+beta)
			               - std::lgamma(alpha)
			               - std::lgamma(beta)
			               + (alpha-1.0)*std::log(x)
			               + (beta-1.0)*std::log(1.0-x) );
		}

		double derivative( Value x ) const {
			double am1 = alpha - 1.0;
			double bm1 = beta - 1.0;
			double onemx = 1.0 - x;
			return std::exp( std::lgamma(alpha+beta)
			               - std::lgamma(alpha)
			               - std::lgamma(beta)
			               + am1*std::log(x)
			               + bm1*std::log(onemx) )
			     * ( am1/x - bm1/onemx );
		}

	};

}
}

#endif
