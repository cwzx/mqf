#ifndef INCLUDED_MQF_DISTRIBUTIONS_NEGATIVE_BINOMIAL
#define INCLUDED_MQF_DISTRIBUTIONS_NEGATIVE_BINOMIAL
#include <random>
#include "../distribution.h"
#include "../functions.h"

namespace mqf {
namespace Distributions {

	struct NegativeBinomial : Mass<uint32_t> {
		uint32_t r;
		double p;
				
		explicit NegativeBinomial( uint32_t r = 2, double p = 0.5 ) : r(r), p(p) {}

		double mean() const {
			return (p * r) / (1.0 - p);
		}

		double mode() const {
			if( r <= 1 ) return 0.0;
			return std::floor( (p * r - p) / (1.0 - p) );
		}

		double variance() const {
			return (p * r) / square(1.0 - p);
		}
		
		double skewness() const {
			return (1.0 + p) / std::sqrt(p * r);
		}

		double excessKurtosis() const {
			return (6.0 + square(1.0 - p) / p ) / r;
		}

		double operator()( Value k ) const {
			auto n = k + r;
			if( n == 0 ) return 0.0;
			return binomial( n - 1, k ) * std::pow( 1.0 - p, r ) * std::pow( p, k );
		}

		std::negative_binomial_distribution<Value> distribution() const {
			return std::negative_binomial_distribution<Value>( r, p );
		}

	};

}

/* mle

x_bar =  p r / (1-p)

1/N sum_i psi(x_i-r) = psi(r) - log(1-p)


1/N sum_i psi(x_i-r) = psi(r) + log(x_bar + r) - log(r)

p = x_bar / (x_bar + r)

*/

}

#endif
