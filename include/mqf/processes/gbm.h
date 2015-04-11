#ifndef INCLUDED_MQF_DISTRIBUTIONS_GBM
#define INCLUDED_MQF_DISTRIBUTIONS_GBM
#include "../process.h"
#include "../distributions/log_normal.h"

namespace mqf {
namespace Processes {
	
	/*
	 * Geometric Brownian Motion
	 *
	 * dX_t = mu X_t dt + sigma X_t dW_t
	 *
	 */
	template<typename Value = double>
	struct GBM : Process<Value> {
		using LogNormal = Distributions::LogNormal;
		double mu, sigma;

		explicit GBM( double drift = 0.5,
		              double volatility = 1.0 ) :
			mu( drift ),
			sigma( volatility )
		{}

		Value drift( const Value& X ) const {
			return mu * X;
		}

		double driftD( const Value& ) const {
			return mu;
		}

		Value diffusion( const Value& X ) const {
			return sigma * X;
		}

		double diffusionD( const Value& ) const {
			return sigma;
		}

		RV<LogNormal> solution( Value inital, Time t ) const {
			return initial * exp( (mu - 0.5*sigma*sigma)*t + sigma * MakeRV(Normal(0,t)) );
		}

	};
		
}
}

#endif
