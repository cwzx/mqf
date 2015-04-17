#ifndef INCLUDED_MQF_COPULAS_HUSLER_REISS
#define INCLUDED_MQF_COPULAS_HUSLER_REISS
#include <cmath>
#include "../distributions/normal.h"

namespace mqf {
namespace Copulas {
	
	struct HuslerReiss {

		double theta;

		explicit HuslerReiss( double theta ) : theta(theta) {}

		double operator()( double u, double v ) const {
			using namespace std;
			Distributions::StdNormal N;
			double r = 0.5 * theta * log( log(u - v) );
			double itheta = 1.0 / theta;
			return pow( u, N.cumulative( itheta + r ) )
			     * pow( v, N.cumulative( itheta - r ) );
		}

	};

}
}

#endif
