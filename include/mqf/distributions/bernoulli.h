#ifndef INCLUDED_MQF_DISTRIBUTIONS_BERNOULLI
#define INCLUDED_MQF_DISTRIBUTIONS_BERNOULLI
#include "binomial.h"

namespace mqf {
namespace Distributions {

	struct Bernoulli : Mass<bool> {
		double p;
				
		explicit Bernoulli( double p = 0.5 ) : p(p) {}
				
		double operator()( Value x ) const {
			return x ? p : 1.0 - p;
		}

		operator Binomial() const {
			return Binomial( 1, p );
		}

		std::bernoulli_distribution distribution() const {
			return std::bernoulli_distribution( p );
		}

	};

}
}

#endif
