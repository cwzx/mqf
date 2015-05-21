#ifndef INCLUDED_MQF_COPULAS_GAUSSIAN
#define INCLUDED_MQF_COPULAS_GAUSSIAN
#include "../distributions/bi_normal.h"
#include "../distributions/normal.h"

namespace mqf {
namespace Copulas {

	struct Gaussian {
		double rho;

		explicit Gaussian( double rho ) : rho(rho) {}

		double operator()( double u, double v ) const {
			Distributions::StdNormal N;
			Distributions::BiStdNormal N2( rho );
			return N2.cumulative( N.inverseCumulative(u), N.inverseCumulative(v) );
		}

		double density( double u, double v ) const {
			Distributions::StdNormal N;
			auto x = N.inverseCumulative(u);
			auto y = N.inverseCumulative(v);
			auto r = 1.0 - rho*rho;
			return std::exp( -0.5 * rho * (rho*(x*x + y*y) - 2.0*x*y) / r ) / std::sqrt(r);
		}

	};

}
}

#endif
