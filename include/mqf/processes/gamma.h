#ifndef INCLUDED_MQF_PROCESSES_GAMMA
#define INCLUDED_MQF_PROCESSES_GAMMA
#include "../process.h"
#include "../distributions/gamma.h"

namespace mqf {

	namespace Processes {
		
		template<typename Value = double>
		struct Gamma : Process<Value> {
			using GammaDist = Distributions::Gamma;

			double k, theta;
			std::mt19937 mt;

			explicit Gamma( double k = 1.0, double theta = 1.0 ) : k(k), theta(theta) {}

			RV<GammaDist> increment( Time dt ) const {
				return MakeRV( GammaDist( k * dt, theta ) );
			}

			RV<GammaDist> solution( Time t ) const {
				return MakeRV( GammaDist( k * t, theta ) );
			}

			Value advance( Value x, Time dt ) const {
				return x + increment( dt ).dist.distribution()( mt );
			}

			double expectation( Time t ) const {
				return t * k * theta;
			}

			double covariance( Time s, Time t ) const {
				return std::fmin(s,t) * k * theta * theta;
			}

			double variance( Time t ) const {
				return t * k * theta * theta;
			}

		};
		
	}

}

#endif
