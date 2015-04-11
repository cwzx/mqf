#ifndef INCLUDED_MQF_DISTRIBUTIONS_POISSON
#define INCLUDED_MQF_DISTRIBUTIONS_POISSON
#include "../random_variable.h"
#include "../distribution.h"
#include "../functions.h"

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
}

#endif
