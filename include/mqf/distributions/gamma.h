#ifndef INCLUDED_MQF_DISTRIBUTIONS_GAMMA
#define INCLUDED_MQF_DISTRIBUTIONS_GAMMA
#include <cassert>
#include <cmath>
#include <random>
#include "../distribution.h"

namespace mqf {
namespace Distributions {

	struct Gamma : Density<> {
		double k, theta;
				
		explicit Gamma( double k = 1.0, double theta = 1.0 ) : k(k), theta(theta) {}

		double mean() const {
			return k*theta;
		}

		double mode() const {
			assert( k >= 1 );
			return (k-1)*theta;
		}

		double variance() const {
			return k*theta*theta;
		}

		double operator()( double x ) const {
			return ( std::pow(x,k-1) * std::exp(-x/theta) ) / ( std::tgamma(k) * std::pow(theta,k) );
		}

		std::gamma_distribution<Value> distribution() const {
			return std::gamma_distribution<Value>( k, theta );
		}

	};

}

/* mle

1/N sum_i log(x_i) = psi(k) + log(theta)
1/N sum_i x_i = k theta

logGeometricMean(x_i) - logMean(x_i) = psi(k) - log(k)
logGeometricMean(x_i) = psi( mean(x_i) / theta ) + log(theta)

*/

	template<typename>
	struct MomentEstimation;

	template<>
	struct MomentEstimation<Distributions::Gamma> {
		using Dist = Distributions::Gamma;
		template<typename It>
		Dist operator()( It p1, It p2 ) const {
			auto mu = sampleMean(p1,p2);
			auto var = sampleVariance(p1,p2,mu);
			auto theta = var / mu;
			auto k = mu / theta;
			return Dist( k, theta );
		}
	};

}

#endif
