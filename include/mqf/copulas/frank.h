#ifndef INCLUDED_MQF_COPULAS_FRANK
#define INCLUDED_MQF_COPULAS_FRANK
#include <cmath>

namespace mqf {
namespace Copulas {
	
	struct Frank {

		double theta;

		explicit Frank( double theta ) : theta(theta) {}

		double operator()( double u, double v ) const {
			return -std::log( 1.0 + (( std::exp(-theta*u) - 1.0 )*( std::exp(-theta*v) - 1.0 ))/( std::exp(-theta) - 1.0 ) ) / theta;
		}

		double generator( double u ) const {
			return -std::log( ( std::exp(-theta * u) - 1.0 )/( std::exp(-theta) - 1.0 ) );
		}

		double generatorInverse( double x ) const {
			return -std::log( 1.0 + std::exp(-x) * ( std::exp(-theta) - 1.0 ) ) / theta;
		}

	};

}
}

#endif
