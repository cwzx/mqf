#ifndef INCLUDED_MQF_DISTRIBUTIONS_CHI
#define INCLUDED_MQF_DISTRIBUTIONS_CHI
#include "chi_squared.h"

namespace mqf {
namespace Distributions {

	struct Chi : Density<> {
		double k;

		explicit Chi( double dof = 2 ) : k(dof) {}

		double mean() const {
			return std::sqrt(2.0) * std::tgamma( (k+1)*0.5 ) / std::tgamma(k*0.5);
		}
				
		double mode() const {
			assert( k >= 1 );
			return std::sqrt(k-1);
		}

		double variance() const {
			double mu = mean();
			return k - mu*mu;
		}

		double operator()( double x ) const {
			if( x < 0.0 ) return 0.0;
			return ( 2.0 * std::pow(x,k-1) * std::exp(-0.5*x*x) )
			     / ( std::pow( std::sqrt(2.0), k ) * std::tgamma(0.5*k) );
		}

		double derivative( double x ) const {
			if( x < 0.0 ) return 0.0;
			return ( 2.0 * std::pow(x,k-2) * std::exp(-0.5*x*x) * ( k - 1.0 - x*x ) )
			     / ( std::pow( std::sqrt(2.0), k ) * std::tgamma(0.5*k) );
		}

	};

	RV<Chi> abs( const RV<StdNormal>& X ) {
		return MakeRV( Chi( 1 ) );
	}

	RV<ChiSquared> square( const RV<Chi>& X ) {
		return MakeRV( ChiSquared( X.dist.k ) );
	}

	RV<Chi> sqrt( const RV<ChiSquared>& X ) {
		return MakeRV( Chi( X.dist.k ) );
	}

}

/* mle

2 * logGeometricMean(x_i) = log(2) + psi(k/2)

*/

}

#endif
