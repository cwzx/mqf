#ifndef INCLUDED_MQF_DISTRIBUTIONS_BINOMIAL
#define INCLUDED_MQF_DISTRIBUTIONS_BINOMIAL
#include <random>
#include "../distribution.h"
#include "../functions.h"

namespace mqf {
namespace Distributions {

	struct Binomial : Mass<uint32_t> {
		double p;
		uint32_t n;
				
		explicit Binomial( uint32_t n, double p = 0.5 ) : n(n), p(p) {}

		double mean() const {
			return n * p;
		}

		double variance() const {
			return n * p*(1.0-p);
		}

		double operator()( Value k ) const {
			if( k > n ) return 0.0;
			return binomial(n,k) * std::pow(p,k) * std::pow( 1.0-p, n-k );
		}

		std::binomial_distribution<Value> distribution() const {
			return std::binomial_distribution<Value>( n, p );
		}

	};

}
}

#endif
