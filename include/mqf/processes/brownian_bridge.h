#ifndef INCLUDED_MQF_PROCESSES_BROWNIAN_BRIDGE
#define INCLUDED_MQF_PROCESSES_BROWNIAN_BRIDGE
#include "../process.h"
#include "../distributions/normal.h"

namespace mqf {
namespace Processes {
	
	struct BrownianBridge : Process<> {
		using Normal = Distributions::Normal;
		Time t1, t2;
		Value x1, x2;

		explicit BrownianBridge( double x1 = 0.0,
		                         double x2 = 0.0,
		                         double t1 = 0.0,
		                         double t2 = 1.0 ) :
			x1( x1 ),
			x2( x2 ),
			t1( t1 ),
			t2( t2 )
		{}

		RV<Normal> solution( Time t ) const {
			return MakeRV( Normal( x1 + (t - t1)*(x2 - x1)/(t2 - t1), ((t2 - t)*(t - t1)) / (t2 - t1) ) );
		}

		Value expectation( Time t ) const {
			return x1 + (t - t1)*(x2 - x1)/(t2 - t1);
		}
		
		Value covariance( Time s, Time t ) const {
			if( t < s )
				return covariance( t, s );
			return ((t2 - t)*(s - t1)) / (t2 - t1);
		}

		Value variance( Time t ) const {
			return ((t2 - t)*(t - t1)) / (t2 - t1);
		}

	};
	
}
}

#endif
