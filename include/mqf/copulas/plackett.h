#ifndef INCLUDED_MQF_COPULAS_PLACKETT
#define INCLUDED_MQF_COPULAS_PLACKETT
#include <cmath>
#include "../functions.h"

namespace mqf {
namespace Copulas {
	
	struct Plackett {
		double theta;

		explicit Plackett( double theta ) : theta(theta) {}

		double operator()( double u, double v ) const {
			auto a = theta - 1.0;
			auto y = 1.0 + a*(u + v);
			return ( y - std::sqrt( y*y - 4.0*u*v*theta*a ) )
			     / ( 2.0 * a );
		}

		double density( double u, double v ) const {
			auto a = theta - 1.0;
			auto y = 1.0 + a*(u + v);
			return -( theta * ( a * v * (2.0 * u - 1.0) - a * u - 1.0 ) )
			     / std::sqrt( cube( square(a*(u+v) + 1.0) - 4.0 * a * theta * u * v ) );
		}

	};

}
}

#endif
