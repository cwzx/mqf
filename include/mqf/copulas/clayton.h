#ifndef INCLUDED_MQF_COPULAS_CLAYTON
#define INCLUDED_MQF_COPULAS_CLAYTON
#include <cmath>
#include "../functions.h"

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

		double density( double u, double v ) const {
			auto ut = std::pow( u, theta );
			auto vt = std::pow( v, theta );
			auto umt = 1.0/u;
			auto vmt = 1.0/v;

			if( umt + vmt <= 1.0 )
				return 0.0;

			auto utm = ut/u;
			auto vtm = vt/v;
			return ( (theta+1.0) * utm * vtm * std::pow( umt + vmt - 1.0, -1.0/theta ) ) / square( vt - ut * (vt-1.0) );
		}

	};

}
}

#endif
