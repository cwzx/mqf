#ifndef INCLUDED_MQF_DISTRIBUTIONS_BERNOULLI
#define INCLUDED_MQF_DISTRIBUTIONS_BERNOULLI
#include "binomial.h"
#include "../stats/mle.h"
#include <algorithm>

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

	template<>
	struct MLE<Distributions::Bernoulli> {
		using Dist = Distributions::Bernoulli;
		template<typename It>
		Dist operator()( It p1, It p2 ) const {
			auto N = std::distance(p1,p2);
			auto N1 = std::count(p1,p2,true);
			return Dist( (double)N1 / N );
		}
	};

}

#endif
