#ifndef INCLUDED_MQF_COPULAS_GAUSSIAN
#define INCLUDED_MQF_COPULAS_GAUSSIAN
#include <cmath>

namespace mqf {
namespace Copulas {

	struct Gaussian {

		double rho;

		explicit Gaussian( double rho ) : rho(rho) {}

		double operator()( double u, double v ) const {
			return 0.0; // todo
		}


	};

}
}

#endif
