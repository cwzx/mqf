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

		double density( double u, double v ) const {
			double ut = std::pow( 1.0 - u, theta );
			double vt = std::pow( 1.0 - v, theta );
			double utm = std::pow( 1.0 - u, theta - 1.0 );
			double vtm = std::pow( 1.0 - v, theta - 1.0 );
			return utm * vtm * std::pow( ut - (ut - 1.0) * vt, 1.0/theta - 2.0 ) * ( theta - (ut - 1.0) * (vt - 1.0) );
		}

	};

}
}

#endif
