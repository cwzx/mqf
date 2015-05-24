#ifndef INCLUDED_MQF_DISTRIBUTIONS_INVERSE_GAMMA
#define INCLUDED_MQF_DISTRIBUTIONS_INVERSE_GAMMA
#include "gamma.h"

namespace mqf {
namespace Distributions {

	struct InverseGamma : Density<> {
		double alpha, beta;
				
		explicit InverseGamma( double alpha = 1.0, double beta = 1.0 ) : alpha(alpha), beta(beta) {}

		double mean() const {
			assert( alpha > 1 );
			return beta / ( alpha - 1.0 );
		}

		double mode() const {
			return beta / ( alpha + 1.0 );
		}

		double variance() const {
			assert( alpha > 2 );
			double am1 = alpha - 1.0;
			return beta*beta / ( am1*am1 * ( alpha - 2.0 ) );
		}

		double operator()( double x ) const {
			return ( std::pow(beta,alpha) * std::exp(-beta/x) )
			     / ( std::tgamma( alpha ) * std::pow( x, alpha + 1 ) );
		}

		double derivative( double x ) const {
			return ( std::pow(beta,alpha) * std::exp(-beta/x) * ( beta - (alpha+1) * x ) )
			     / ( std::tgamma(alpha) * std::pow(x,alpha+3) );
		}

	};

	RV<InverseGamma> operator*( const RV<InverseGamma>& lhs, double rhs ) {
		return MakeRV( InverseGamma( lhs.dist.alpha, lhs.dist.beta * rhs ) );
	}

	RV<InverseGamma> operator*( double lhs, const RV<InverseGamma>& rhs ) {
		return rhs * lhs;
	}

	RV<InverseGamma> operator/( double lhs, const RV<Gamma>& rhs ) {
		return MakeRV( InverseGamma( rhs.dist.k, lhs / rhs.dist.theta ) );
	}

}

/* mle

1/N sum_i log(x_i) = log(b) - psi(a)

harmonicMean(x_i) = b/a


logGeometricMean(x_i) - logHarmonicMean(x_i) = log(a) - psi(a)

logGeometricMean(x_i) = log(b) - psi( b / harmonicMean(x_i) )

*/

}

#endif
