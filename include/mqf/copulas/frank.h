#ifndef INCLUDED_MQF_COPULAS_FRANK
#define INCLUDED_MQF_COPULAS_FRANK
#include <cmath>
#include "../functions.h"

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

		double density( double u, double v ) const {
			auto a = std::exp( theta );
			auto b = std::exp( theta*u );
			auto c = std::exp( theta*v );
			auto d = b*c;
			return ( theta * (a-1.0) * a * d ) / square( d - a*(b+c+1.0) );
		}

	};

}
}

#endif
