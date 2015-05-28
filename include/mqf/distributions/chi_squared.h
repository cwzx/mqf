#ifndef INCLUDED_MQF_DISTRIBUTIONS_CHI_SQUARED
#define INCLUDED_MQF_DISTRIBUTIONS_CHI_SQUARED
#include "normal.h"
#include "inverse_gamma.h"

namespace mqf {
namespace Distributions {

	struct ChiSquared : Density<> {
		double k;

		explicit ChiSquared( double dof = 2 ) : k(dof) {}

		double mean() const {
			return k;
		}

		double median() const {
			double y = 1.0 - 2.0 / (9.0 * k);
			return k * y*y*y;
		}

		double mode() const {
			return std::fmax(k-2,0);
		}

		double variance() const {
			return 2.0 * k;
		}

		double operator()( double x ) const {
			if( x < 0 )	return 0.0;
			double p = 0.5 * k;
			return ( std::pow( x, p-1 ) * std::exp( -0.5 * x ) )
			     / ( std::pow(2.0,p) * std::tgamma(p) );
		}

		double derivative( double x ) const {
			if( x < 0 ) {
				return 0.0;
			}
			double p = 0.5 * k;
			double xhalf = 0.5 * x;
			double pm1 = p - 1.0;
			return ( std::pow( x, pm1 ) * std::exp( -xhalf ) * ( pm1 - xhalf ) )
			     / ( std::pow(2,p) * std::tgamma(p) );
		}

		std::chi_squared_distribution<Value> distribution() const {
			return std::chi_squared_distribution<Value>( k );
		}

	};

	// Assuming independent
	RV<ChiSquared> operator+( RV<ChiSquared> lhs, RV<ChiSquared> rhs ) {
		return MakeRV( ChiSquared( lhs.dist.k + rhs.dist.k ) );
	}

	// Assuming independent
	RV<ChiSquared> operator*( RV<StdNormal>, RV<StdNormal> ) {
		return MakeRV( ChiSquared( 1 ) );
	}

	RV<Gamma> operator*( RV<ChiSquared> lhs, double rhs ) {
		assert( rhs > 0 );
		return MakeRV( Gamma( 0.5 * lhs.dist.k, 2.0 * rhs ) );
	}

	RV<Gamma> operator*( double lhs, RV<ChiSquared> rhs ) {
		return rhs * lhs;
	}

	RV<Gamma> operator/( RV<ChiSquared> lhs, double rhs ) {
		assert( rhs > 0 );
		return MakeRV( Gamma( 0.5 * lhs.dist.k, 2.0 / rhs ) );
	}

	RV<InverseGamma> operator/( double lhs, RV<ChiSquared> rhs ) {
		return MakeRV( InverseGamma( 0.5 * rhs.dist.k, 0.5 * lhs ) );
	}

}

/* mle

psi(k/2) = 1/N sum_i log(x_i) - log(2)

*/


}

#endif
