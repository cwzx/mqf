#ifndef INCLUDED_MQF_DISTRIBUTIONS_GAMMA
#define INCLUDED_MQF_DISTRIBUTIONS_GAMMA
#include "../random_variable.h"
#include "../distribution.h"
#include <cassert>
#include <cmath>

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
}

#endif
