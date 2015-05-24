#ifndef INCLUDED_MQF_DISTRIBUTIONS_INVERSE_CHI_SQUARED
#define INCLUDED_MQF_DISTRIBUTIONS_INVERSE_CHI_SQUARED
#include "inverse_gamma.h"

namespace mqf {
namespace Distributions {

	struct InverseChiSquared : Density<> {
		double k;

		explicit InverseChiSquared( double dof = 1 ) : k(dof) {}

		double mean() const {
			assert( k > 2 );
			return 1.0 / (k - 2);
		}

		double mode() const {
			return 1.0 / (k + 2);
		}

		double variance() const {
			assert( k > 4 );
			double km2 = k - 2.0;
			return 2.0 / ( km2*km2 * (k-4) );
		}

		double operator()( double x ) const {
			double p = 0.5 * k;
			return std::exp( -( (p+1)*std::log(x) + p*std::log(2.0) + std::lgamma(p) + 0.5/x ) );
		}

		double derivative( double x ) const {
			double p = 0.5 * k;
			double pp1 = p + 1.0;
			return std::exp( -( pp1*std::log(x) + p*std::log(2.0) + std::lgamma(p) + 0.5/x ) )
				* ( 0.5 - pp1*x ) / (x*x);
		}

		operator InverseGamma() const {
			return InverseGamma( 0.5 * k, 0.5 );
		}

	};

}

/* mle

1/N sum_i log(x_i) = -psi(k/2) - log(2)

*/

}

#endif
