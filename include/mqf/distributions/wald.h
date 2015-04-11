#ifndef INCLUDED_MQF_DISTRIBUTIONS_WALD
#define INCLUDED_MQF_DISTRIBUTIONS_WALD
#include "normal.h"

namespace mqf {
namespace Distributions {

	struct Wald : Density<> {
		double lambda, mu;
				
		explicit Wald( double lambda = 1.0, double mu = 1.0 ) : lambda(lambda), mu(mu) {}

		double mean() const {
			return mu;
		}

		double mode() const {
			double r = 1.5 * mu / lambda;
			return mu * ( std::sqrt( 1.0 + r*r ) - r );
		}

		double variance() const {
			return mu*mu*mu/lambda;
		}

		double skewness() const {
			return 3.0 * std::sqrt( mu / lambda );
		}

		double excessKurtosis() const {
			return 15.0 * mu / lambda;
		}

		double operator()( double x ) const {
			double y = ( x - mu );
			return std::sqrt( lambda / ( 2.0 * Pi * x*x*x ) ) * std::exp( -0.5 * lambda * y*y / ( mu*mu * x ) );
		}

		double cumulative( double x ) const {
			double y = ( x - mu );
			double a = std::sqrt( lambda / x );
			double b = x / mu; 
			StdNormal N;
			return N.cumulative( a * (b - 1) ) + std::exp( 2.0 * lambda/mu ) * N.cumulative( -a * (b + 1) );
		}

	};

}
}

#endif
