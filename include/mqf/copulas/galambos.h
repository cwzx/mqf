#ifndef INCLUDED_MQF_COPULAS_GALAMBOS
#define INCLUDED_MQF_COPULAS_GALAMBOS
#include <cmath>

namespace mqf {
namespace Copulas {
	
	struct Galambos {

		double theta;

		explicit Galambos( double theta ) : theta(theta) {}

		double operator()( double u, double v ) const {
			using namespace std;
			return u*v * exp( pow( pow(-log(u),-theta) + pow(-log(v),-theta), -1.0/theta ) );
		}

	};

}
}

#endif
