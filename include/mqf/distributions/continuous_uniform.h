#ifndef INCLUDED_MQF_DISTRIBUTIONS_CONTINUOUS_UNIFORM
#define INCLUDED_MQF_DISTRIBUTIONS_CONTINUOUS_UNIFORM
#include "../distribution.h"

namespace mqf {
namespace Distributions {

	struct ContinuousUniform : Density<> {
		Value a, b;
				
		explicit ContinuousUniform( Value a = 0.0, Value b = 1.0 ) : a(a), b(b) {}

		Value mean() const {
			return 0.5 * (a + b);
		}

		Value median() const {
			return 0.5 * (a + b);
		}

		Value variance() const {
			double ba = b - a;
			return ba * ba / 12.0;
		}

		double operator()( Value x ) const {
			if( x < a || x > b )
				return 0.0;
			return 1.0 / (b-a);
		}

		double derivative( Value ) const {
			return 0.0;
		}

		double cumulative( Value x ) const {
			if( x < a )
				return 0.0;
			if( x >= b )
				return 1.0;
			return (x-a) / (b-a);
		}

		Value cumulativeInverse( double p ) const {
			return a + p*(b - a);
		}

		std::uniform_real_distribution<Value> distribution() const {
			return std::uniform_real_distribution<Value>( a, b );
		}

	};

}
}

#endif
