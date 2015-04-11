#ifndef INCLUDED_MQF_PROCESSES_WIENER
#define INCLUDED_MQF_PROCESSES_WIENER
#include "../process.h"
#include "../distributions/log_normal.h"

namespace mqf {
namespace Processes {
	
	template<typename Value = double>
	struct Wiener : Process<Value> {

		Value drift( const Value& ) const {
			return 0.0;
		}

		double driftD( const Value& ) const {
			return 0.0;
		}

		Value diffusion( const Value& ) const {
			return 1.0;
		}

		double diffusionD( const Value& ) const {
			return 0.0;
		}

		RV<LogNormal> solution( Value inital, Time t ) const {
			return MakeRV( Normal( 0, t ) );
		}

		LogNormal distribution( Value inital, Time t ) const {
			return solution( initial, t ).dist;
		}

		double expectation( Time ) const {
			return 0.0;
		}

		double covariance( Time s, Time t ) const {
			return std::fmin(s,t);
		}

		double quadraticVariation( Time t ) const {
			return t;
		}

		RV<Normal> increment( Time dt ) const {
			return MakeRV( Normal( 0.0, dt ) );
		}

		RV<Normal> increment( Time s, Time t ) const {
			return increment( t-s );
		}

	};
	
}
}

#endif
