#ifndef INCLUDED_MQF_DISTRIBUTIONS_HALF_LOGISTIC
#define INCLUDED_MQF_DISTRIBUTIONS_HALF_LOGISTIC
#include "logistic.h"
#include "../functions.h"

namespace mqf {
namespace Distributions {

	struct HalfStdLogistic : Density<> {

		double mean() const {
			return std::log( 4.0 );
		}

		double median() const {
			return std::log( 3.0 );
		}

		double mode() const {
			return 0.0;
		}

		double variance() const {
			return Pi*Pi / 3.0 - square( std::log(4.0) );
		}

		double operator()( double x ) const {
			auto y = std::exp( -x );
			return 2.0 * y / square(1.0 + y);
		}

		double cumulative( double x ) const {
			auto y = std::exp( -x );
			return (1.0 - y) / (1.0 + y);
		}

	};

	RV<HalfStdLogistic> abs( RV<StdLogistic> ) {
		return MakeRV( HalfStdLogistic() );
	}

}
}

#endif
