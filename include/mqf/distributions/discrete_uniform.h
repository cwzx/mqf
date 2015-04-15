#ifndef INCLUDED_MQF_DISTRIBUTIONS_DISCRETE_UNIFORM
#define INCLUDED_MQF_DISTRIBUTIONS_DISCRETE_UNIFORM
#include <random>
#include "../distribution.h"

namespace mqf {
namespace Distributions {

	struct DiscreteUniform : Mass<> {
		Value a, b;
				
		explicit DiscreteUniform( Value a = 0, Value b = 1 ) : a(a), b(b) {}

		double mean() const {
			return 0.5 * (a + b);
		}

		double median() const {
			return 0.5 * (a + b);
		}

		double variance() const {
			Value ba = b - a + 1;
			return (ba * ba - 1) / 12.0;
		}

		double operator()( Value x ) const {
			if( x < a || x > b )
				return 0.0;
			return 1.0 / (b-a+1);
		}

		double cumulative( Value x ) const {
			if( x < a )
				return 0.0;
			if( x >= b )
				return 1.0;
			return ( x - a + 1 ) / ( b - a + 1 );
		}

		std::uniform_int_distribution<Value> distribution() const {
			return std::uniform_int_distribution<Value>( a, b );
		}

	};

}
}

#endif
