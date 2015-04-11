#ifndef INCLUDED_MQF_DISTRIBUTIONS_F
#define INCLUDED_MQF_DISTRIBUTIONS_F
#include "../random_variable.h"
#include "../distribution.h"
#include <cmath>
#include <cassert>

namespace mqf {
namespace Distributions {

	struct F : Density<> {
		double d1, d2;
				
		explicit F( double d1 = 2.0, double d2 = 2.0 ) : d1(d1), d2(d2) {}

		double mean() const {
			assert( d2 > 2 );
			return d2 / (d2 - 2);
		}

		double mode() const {
			assert( d1 > 2 );
			return (d2 * (d1 - 2)) / (d1 * (d2 + 2));
		}

		double variance() const {
			assert( d2 > 4 );
			double a = d2 - 2;
			return ( 2.0 * d2*d2 * (d1 + a) ) / ( d1*a*a*(d2 - 4) );
		}

		double skewness() const {
			assert( d2 > 6 );
			double a = d2 - 2;
			return ( 2.0 * d1 + a ) / (d2 - 6) * std::sqrt( (8.0 * (d2 - 4)) / ( d1*(d1 + a) ) );
		}

		double excessKurtosis() const {
			assert( d2 > 8 );
			double a = d2 - 2;
			return 12.0 * ( d1*(5*d2 - 22)*(d1 + a) + (d2 - 4)*a*a )
			            / ( d1*(d2 - 6)*(d2 - 8)*(d1 + a) );
		}

		double operator()( double x ) const {
			double halfd1 = 0.5 * d1;
			double halfd2 = 0.5 * d2;
			double r = d1 / d2;
			return std::exp( (halfd1 - 1.0)*std::log(x) - (halfd1+halfd2)*std::log(1.0 + r*x) - logBeta( halfd1, halfd2 ) + halfd1 * std::log(r) );
		}

		std::fisher_f_distribution<Value> distribution() const {
			return std::fisher_f_distribution<Value>( d1, d2 );
		}

	};

}
}

#endif
