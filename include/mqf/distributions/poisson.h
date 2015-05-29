#ifndef INCLUDED_MQF_DISTRIBUTIONS_POISSON
#define INCLUDED_MQF_DISTRIBUTIONS_POISSON
#include <random>
#include "../distribution.h"
#include "../functions.h"
#include "../stats/mle.h"
#include "../stats/descriptive.h"

namespace mqf {
namespace Distributions {

	struct Poisson : Mass<uint32_t> {
		double lambda;

		explicit Poisson( double lambda = 1.0 ) : lambda(lambda) {}

		double mean() const {
			return lambda;
		}

		// approximation
		double median() const {
			return std::floor( lambda + 1.0/3.0 - 0.02 / lambda );
		}

		double mode() const {
			return std::floor(lambda);
		}

		double variance() const {
			return lambda;
		}

		double operator()( uint32_t k ) const {
			return std::pow(lambda,k)/factorial(k) * std::exp(-lambda);
		}
		
		std::poisson_distribution<Value> distribution() const {
			return std::poisson_distribution<Value>( lambda );
		}

	};

}

	template<>
	struct MLE<Distributions::Poisson> {
		using Dist = Distributions::Poisson;
		template<typename It>
		Dist operator()( It p1, It p2 ) const {
			return Dist( sampleMean(p1,p2) );
		}
	};
	
	template<typename>
	struct MomentEstimation;

	template<>
	struct MomentEstimation<Distributions::Poisson> {
		using Dist = Distributions::Poisson;
		template<typename It>
		Dist operator()( It p1, It p2 ) const {
			return Dist( sampleMean(p1,p2) );
		}
	};

}

#endif
