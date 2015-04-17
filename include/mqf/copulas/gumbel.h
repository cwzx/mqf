#ifndef INCLUDED_MQF_COPULAS_GUMBEL
#define INCLUDED_MQF_COPULAS_GUMBEL
#include <cmath>

namespace mqf {
namespace Copulas {
	
	struct Gumbel {

		double theta;

		explicit Gumbel( double theta ) : theta(theta) {}

		double operator()( double u, double v ) const {
			return std::exp( -std::pow( std::pow( -std::log(u),theta) + std::pow( -std::log(v),theta), 1.0/theta ) );
		}

		double generator( double u ) const {
			return std::pow( -std::log(u), theta );
		}

		double generatorInverse( double x ) const {
			return std::exp( -std::pow(x,1.0/theta) );
		}

	};

}
}

#endif
