#ifndef INCLUDED_MQF_DISTRIBUTIONS_WEIBULL
#define INCLUDED_MQF_DISTRIBUTIONS_WEIBULL
#include <cmath>
#include <random>
#include "../distribution.h"
#include "../functions.h"

namespace mqf {
namespace Distributions {

	struct Weibull : Density<> {
		double k, lambda;
				
		explicit Weibull( double k = 1.0, double lambda = 1.0 ) : k(k), lambda(lambda) {}

		double mean() const {
			return lambda * std::tgamma( 1.0 + 1.0/k );
		}

		double median() const {
			return lambda * std::pow( std::log( 2.0 ), 1.0/k );
		}

		double mode() const {
			if( k == 1 )
				return 0.0;
			double r = 1.0 / k;
			return lambda * std::pow( 1.0 - r, r );
		}

		double variance() const {
			double rk = 1.0 / k;
			return lambda*lambda * ( std::tgamma( 1.0 + 2.0*rk ) - square( std::tgamma( 1.0 + rk ) ) );
		}

		double skewness() const {
			double mu = mean();
			double sigma2 = variance();
			return ( std::tgamma(1.0+3.0/k) * lambda*lambda*lambda - 3.0*mu*sigma2 - mu*mu*mu ) / ( sigma2 * std::sqrt(sigma2) );
		}

		double operator()( double x ) const {
			double y = x / lambda;
			double r = std::pow( y, k-1 );
			return k * r * std::exp( -r*y ) / lambda;
		}

		double log( double x ) const {
			double y = x / lambda;
			return std::log( k / lambda ) + (k-1) * std::log( y ) - std::pow( y, k );
		}

		double cumulative( double x ) const {
			return 1.0 - std::exp( -std::pow( x / lambda, k ) );
		}

		std::weibull_distribution<Value> distribution() const {
			return std::weibull_distribution<Value>( k, lambda );
		}

	};

}
}

#endif
