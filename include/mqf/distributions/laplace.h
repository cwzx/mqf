#ifndef INCLUDED_MQF_DISTRIBUTIONS_LAPLACE
#define INCLUDED_MQF_DISTRIBUTIONS_LAPLACE
#include "../random_variable.h"
#include "../distribution.h"
#include <cmath>

namespace mqf {
namespace Distributions {

	struct Laplace : Density<> {
		double mu, b;
				
		explicit Laplace( double mu = 0.0, double b = 1.0 ) : mu(mu), b(b) {}

		double mean() const {
			return mu;
		}

		double median() const {
			return mu;
		}

		double mode() const {
			return mu;
		}

		double variance() const {
			return 2.0 * b * b;
		}

		double operator()( Value x ) const {
			return std::exp(-std::fabs(x-mu)/b) / (2.0*b);
		}

		double cumulative( Value x ) const {
			double y = (x - mu) / b;
			if( x < mu ) {
				return 0.5 * std::exp(y);
			}
			return 1.0 - 0.5 * std::exp(-y);
		}
	};

	RV<Laplace> operator+( const RV<Laplace>& lhs, double rhs ) {
		return MakeRV( Laplace( lhs.dist.mu + rhs, lhs.dist.b ) );
	}

	RV<Laplace> operator+( double lhs, const RV<Laplace>& rhs ) {
		return rhs + lhs;
	}

	RV<Laplace> operator*( const RV<Laplace>& lhs, double rhs ) {
		return MakeRV( Laplace( lhs.dist.mu * rhs, lhs.dist.b * rhs ) );
	}

	RV<Laplace> operator*( double lhs, const RV<Laplace>& rhs ) {
		return rhs * lhs;
	}

}
}

#endif
