#ifndef INCLUDED_MQF_PROCESSES_BROWNIAN_BRIDGE
#define INCLUDED_MQF_PROCESSES_BROWNIAN_BRIDGE
#include "../process.h"
#include <random>

namespace mqf {

	namespace Processes {
		
		struct BrownianBridge : Process<> {
			double T, end;
			std::mt19937 mt;

			explicit BrownianBridge( double T = 1.0, double end = 0.0 ) : T(T), end(end) {}

			Value advance( const Value& x, Time t, Time dt ) {
				double alpha = dt / ( T - t );
				std::normal_distribution<> normal( 0.0, dt * (1.0-alpha) );
				return x + alpha * (end - x) + normal(mt);
			}
		};
		
	}

}

#endif
