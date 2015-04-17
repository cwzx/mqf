#ifndef INCLUDED_MQF_COPULAS_CLAYTON
#define INCLUDED_MQF_COPULAS_CLAYTON
#include <cmath>

namespace mqf {
namespace Copulas {

	struct Clayton {

		double theta;

		explicit Clayton( double theta ) : theta(theta) {}

		double operator()( double u, double v ) const {
			return std::pow( std::fmax( std::pow(u,-theta) + std::pow(v,-theta) - 1.0, 0.0 ), -1.0/theta );
		}

		double generator( double u ) const {
			return ( std::pow(u,-theta) - 1.0 ) / theta;
		}

		double generatorInverse( double x ) const {
			return std::pow( 1.0 + theta*x, -1.0/theta );
		}

	};

}
}

#endif
