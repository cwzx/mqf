#ifndef INCLUDED_MQF_COPULAS_MARSHALL_OLKIN
#define INCLUDED_MQF_COPULAS_MARSHALL_OLKIN
#include <cmath>

namespace mqf {
namespace Copulas {
	
	struct MarshallOlkin {

		double a, b;

		explicit MarshallOlkin( double a, double b ) : a(a), b(b) {}

		double operator()( double u, double v ) const {
			return std::fmin( v*std::pow(u,a), u*std::pow(v,b) );
		}

	};

}
}

#endif
