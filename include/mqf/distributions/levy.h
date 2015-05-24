#ifndef INCLUDED_MQF_DISTRIBUTIONS_LEVY
#define INCLUDED_MQF_DISTRIBUTIONS_LEVY
#include <cmath>
#include "../distribution.h"
#include "../random_variable.h"
#include "../constants.h"

namespace mqf {
namespace Distributions {

	struct Levy : Density<> {
		double mu, c;
				
		explicit Levy( double mu = 0.0, double c = 1.0 ) : mu(mu), c(c) {}

		double operator()( double x ) const {
			double y = (x - mu) / c;
			return exp(-0.5 / y) / ( c * y * sqrt(2.0*Pi*y) );
		}

		double derivative( double x ) const {
			double y = (x - mu) / c;
			return ( exp(-0.5 / y) * ( 1.0 - 3.0 * y ) ) / ( 2.0 * c*c * y*y*y * sqrt(2.0*Pi*y) );
		}

		double cumulative( double x ) const {
			return std::erfc( sqrt( c / (2.0*(x-mu)) ) );
		}

	};

	struct StdLevy : Density<> {

		double operator()( double x ) const {
			return 1.0 / ( x * sqrt(2.0*Pi*x) ) * exp(-0.5 / x);
		}

		double derivative( double x ) const {
		}

		double cumlative( double x ) const {
			return std::erfc( sqrt( 1.0 / (2.0*x) ) );
		}

		operator Levy() const {
			return Levy( 0, 1 );
		}

	};

	RV<Levy> operator*( const RV<Levy>& lhs, double rhs ) {
		return MakeRV( Levy( lhs.dist.mu * rhs, lhs.dist.c * rhs ) );
	}

	RV<Levy> operator*( double lhs, const RV<Levy>& rhs ) {
		return rhs * lhs;
	}

	RV<Levy> operator+( const RV<Levy>& lhs, double rhs ) {
		return MakeRV( Levy( lhs.dist.mu + rhs, lhs.dist.c ) );
	}

	RV<Levy> operator+( double lhs, const RV<Levy>& rhs ) {
		return rhs + lhs;
	}

}

/* mle

1/N sum 1/(x_i - mu)^2 = 3 ( 1/N sum 1/(x_i - mu) )^2

c = harmonicMean(x_i - mu)

*/

}

#endif
