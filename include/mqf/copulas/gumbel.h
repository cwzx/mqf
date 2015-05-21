#ifndef INCLUDED_MQF_COPULAS_GUMBEL
#define INCLUDED_MQF_COPULAS_GUMBEL
#include <cmath>
#include "../functions.h"

namespace mqf {
namespace Copulas {
	
	struct Gumbel {
		double theta;

		explicit Gumbel( double theta ) : theta(theta) {}

		double operator()( double u, double v ) const {
			using namespace std;
			return exp( -pow( pow(-log(u),theta) + pow(-log(v),theta), 1.0/theta ) );
		}

		double generator( double u ) const {
			return std::pow( -std::log(u), theta );
		}

		double generatorInverse( double x ) const {
			return std::exp( -std::pow(x,1.0/theta) );
		}

		double density( double u, double v ) const {
			using namespace std;
			auto logu = log(u);
			auto logv = log(v);
			auto logut = pow( -logu, theta );
			auto logvt = pow( -logv, theta );
			auto logutm = pow( -logu, theta - 1.0 );
			auto logvtm = pow( -logv, theta - 1.0 );
			auto y = pow( logut + logvt, 1.0/theta );

			return ( logutm * logvtm * (theta + y - 1.0) * exp( -y ) * y )
			     / ( u*v * square( logut + logvt ) );
		}
		
	};

}
}

#endif
