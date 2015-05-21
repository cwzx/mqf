#ifndef INCLUDED_MQF_COPULAS_ALI_MIKHAIL_HAQ
#define INCLUDED_MQF_COPULAS_ALI_MIKHAIL_HAQ
#include <cmath>

namespace mqf {
namespace Copulas {
	
	struct AliMikhailHaq {
		double theta;

		explicit AliMikhailHaq( double theta ) : theta(theta) {}

		double operator()( double u, double v ) const {
			return ( u*v ) / (1.0 - theta * (1.0 - u) * (1.0 - v) );
		}

		double generator( double u ) const {
			return std::log( ( 1.0 - theta * (1.0 - u) ) / u );
		}

		double generatorInverse( double x ) const {
			return (1.0 - theta) / (std::exp(x) - theta);
		}

		double density( double u, double v ) const {
			return ( theta*theta * (u + v - u*v - 1.0) - theta * (u + v + u*v - 2.0) - 1.0 )
			     / cube( theta * (u - 1.0) * (v - 1.0) - 1.0 );
		}

	};

}
}

#endif
