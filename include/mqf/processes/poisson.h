#ifndef INCLUDED_MQF_PROCESSES_POISSON
#define INCLUDED_MQF_PROCESSES_POISSON
#include "../process.h"
#include "../distributions/poisson.h"

namespace mqf {

	namespace Processes {
		
		template<typename Value = uint32_t>
		struct Poisson : Process<Value> {
			using Pois = Distributions::Poisson;

			double lambda;

			explicit Poisson( double lambda = 1.0 ) : lambda(lambda) {}

			RV<Pois> increment( Time dt ) const {
				return MakeRV( Pois( lambda * dt ) );
			}
			
			RV<Pois> solution( Time t ) const {
				return MakeRV( Pois( lambda * t ) );
			}

		};
		
	}

}

#endif
