#ifndef INCLUDED_MQF_LIKELIHOOD
#define INCLUDED_MQF_LIKELIHOOD
#include "../distributions/normal.h"

namespace mqf {
namespace LogLikelihood {

	struct NormalMean {
		double sigma2, x;

		NormalMean( double sigma2, double x ) : sigma2(sigma2), x(x) {}

		double operator()( double mu ) const {
			return Distributions::Normal( mu, sigma2 ).log(x);
		}

	};

	struct NormalVariance {
		double mu, x;

		NormalVariance( double mu, double x ) : mu(mu), x(x) {}

		double operator()( double sigma2 ) const {
			return Distributions::Normal( mu, sigma2 ).log(x);
		}

	};

}
}

#endif
