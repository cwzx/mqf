#ifndef INCLUDED_MQF_PROCESSES_SQUARE_ROOT
#define INCLUDED_MQF_PROCESSES_SQUARE_ROOT
#include "../process.h"
#include "../distributions/log_normal.h"

namespace mqf {
namespace Processes {

	/*
	 * Square-root process
	 *
	 * dX_t = theta ( mu - X_t ) dt + sigma sqrt(X_t) dW_t
	 *
	 */
	struct SquareRoot : Process<double> {
		double mu, theta, sigma;

		explicit SquareRoot( double mean = 1.0,
			                 double reversion = 1.0,
			                 double volatility = 1.0 ) :
			mu(mean),
			theta(reversion),
			sigma(volatility)
		{}

		double drift( double X ) const {
			return theta * ( mu - X );
		}

		double driftD( double ) const {
			return -theta;
		}

		double diffusion( double X ) const {
			return sigma * std::sqrt( X );
		}

		double diffusionD( double X ) const {
			return 0.5 * sigma / std::sqrt( X );
		}

		double expectation( double initial, Time t ) const {
			double y = std::exp( -theta * t );
			return initial * y + mu * (1.0-y);
		}

		double expectationLimit() const {
			return mu;
		}

		double variance( double initial, Time t ) const {
			double y = std::exp( -theta * t );
			double z = 1.0 - y;
			return ( sigma*sigma * z / theta ) * ( initial * y + 0.5 * mu * z );
		}

		double varianceLimit() const {
			return 0.5 * mu * sigma*sigma / theta;
		}

	};

	using CIR = SquareRoot;
	
}
}

#endif
