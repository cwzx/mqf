#ifndef INCLUDED_MQF_COPULAS_PLACKETT
#define INCLUDED_MQF_COPULAS_PLACKETT
#include <cmath>

namespace mqf {
namespace Copulas {
	
	struct Plackett {

		double theta;

		explicit Plackett( double theta ) : theta(theta) {}

		double operator()( double u, double v ) const {
			using namespace std;
			double thetam1 = theta - 1.0;
			double y = 1.0 + thetam1*(u + v);
			return ( y - sqrt( y*y - 4.0*u*v*theta*thetam1 ) )
			     / ( 2.0 * thetam1 );
		}

	};

}
}

#endif
