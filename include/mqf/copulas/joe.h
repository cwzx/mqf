#ifndef INCLUDED_MQF_COPULAS_JOE
#define INCLUDED_MQF_COPULAS_JOE
#include <cmath>

namespace mqf {
namespace Copulas {
	
	struct Joe {

		double theta;

		explicit Joe( double theta ) : theta(theta) {}

		double operator()( double u, double v ) const {
			double a = std::pow( 1.0 - u, theta );
			double b = std::pow( 1.0 - v, theta );
			return 1.0 - std::pow( a + b - a*b, 1.0/theta );
		}

		double generator( double u ) const {
			double a = std::pow( 1.0 - u, theta );
			return -std::log( 1.0 - a );
		}

		double generatorInverse( double x ) const {
			return 1.0 - std::pow( 1.0 - std::exp(-x), 1.0/theta );
		}

	};

}
}

#endif
