#ifndef INCLUDED_MQF_DISTRIBUTIONS_BETA
#define INCLUDED_MQF_DISTRIBUTIONS_BETA
#include <cassert>
#include "../distribution.h"
#include "../functions.h"

namespace mqf {
namespace Distributions {

	struct Beta : Density<> {
		double alpha, beta;

		explicit Beta( double alpha = 1.0, double beta = 1.0 ) : alpha(alpha), beta(beta) {}

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

		double skewness() const {
			double sum = alpha + beta;
			return 2.0*( beta - alpha ) / ( sum + 2.0 ) * std::sqrt( (sum + 1.0)/( alpha * beta ) );
		}

		double operator()( Value x ) const {
			return std::exp( -logBeta(alpha,beta)
			               + (alpha-1.0)*std::log(x)
			               + (beta-1.0)*std::log(1.0-x) );
		}

		double derivative( Value x ) const {
			double am1 = alpha - 1.0;
			double bm1 = beta - 1.0;
			double onemx = 1.0 - x;
			return std::exp( -logBeta(alpha,beta)
			               + am1*std::log(x)
			               + bm1*std::log(onemx) )
			     * ( am1/x - bm1/onemx );
		}

		double log( Value x ) const {
			return -logBeta(alpha,beta)
			     + (alpha-1.0)*std::log(x)
			     + (beta-1.0)*std::log(1.0-x);
		}

	};

}

}

#endif
