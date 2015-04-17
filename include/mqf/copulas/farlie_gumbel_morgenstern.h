#ifndef INCLUDED_MQF_COPULAS_FARLIE_GUMBEL_MORGENSTERN
#define INCLUDED_MQF_COPULAS_FARLIE_GUMBEL_MORGENSTERN
#include <cmath>

namespace mqf {
namespace Copulas {
	
	struct FarlieGumbelMorgenstern {

		double theta;

		explicit FarlieGumbelMorgenstern( double theta ) : theta(theta) {}

		double operator()( double u, double v ) const {
			return u*v * ( 1.0 + theta*(1.0 - u)*(1.0 - v) );
		}

		double density( double u, double v ) const {
			return 1.0 + theta*(1.0 - 2.0*u)*(1.0 - 2.0*v);
		}


	};

}
}

#endif
