#ifndef INCLUDED_MQF_DISTRIBUTIONS_GEOMETRIC
#define INCLUDED_MQF_DISTRIBUTIONS_GEOMETRIC
#include "exponential.h"
#include "negative_binomial.h"

namespace mqf {
namespace Distributions {

	struct Geometric : Mass<uint32_t> {
		double p;
				
		explicit Geometric( double p = 0.5 ) : p(p) {}

		double mean() const {
			return (1.0-p)/p;
		}

		double median() const {
			return std::ceil(-1.0/std::log2(1.0-p)) - 1.0;
		}

		double mode() const {
			return 0.0;
		}

		double variance() const {
			return (1.0 - p)/(p*p);
		}

		double skewness() const {
			return (2.0-p)/std::sqrt(1.0-p);
		}

		double excessKurtosis() const {
			return 6.0 + (p*p)/(1.0-p);
		}

		double operator()( Value k ) const {
			return std::pow(1.0-p,k) * p;
		}

		double cumulative( Value k ) const {
			return 1.0 - std::pow(1.0-p,k+1);
		}

		std::geometric_distribution<Value> distribution() const {
			return std::geometric_distribution<Value>( p );
		}

		operator NegativeBinomial() const {
			return NegativeBinomial( 1, 1.0 - p );
		}

	};

	RV<Geometric> floor( const RV<Exponential>& x ) {
		return MakeRV( Geometric( 1.0 - std::exp(-x.dist.lambda) ) );
	}

}

	template<>
	struct MLE<Distributions::Geometric> {
		using Dist = Distributions::Geometric;
		template<typename It>
		Dist operator()( It p1, It p2 ) const {
			return Dist( 1.0 / ( 1.0 + sampleMean(p1,p2) ) );
		}
	};

}

#endif
