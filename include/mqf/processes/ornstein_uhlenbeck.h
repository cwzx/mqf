#ifndef INCLUDED_MQF_ORNSTEIN_UHLENBECK
#define INCLUDED_MQF_ORNSTEIN_UHLENBECK
#include "../process.h"
#include "../distributions/log_normal.h"

namespace mqf {
namespace Processes {

	/*
	 * Ornstein--Uhlenbeck process
	 *
	 * dX_t = theta ( mu - X_t ) dt + sigma dW_t
	 *
	 */
	struct OrnsteinUhlenbeck : Process<double> {
		using Normal = Distributions::Normal;

		double mu, theta, sigma;

		explicit OrnsteinUhlenbeck( double mean = 1.0,
			                        double reversion = 1.0,
			                        double volatility = 1.0 ) :
			mu(mean),
			theta(reversion),
			sigma(volatility)
		{}

		Value drift( const Value& X ) const {
			return theta * ( mu - X );
		}

		Value driftD( const Value& X ) const {
			return -theta * X;
		}

		Value diffusion( const Value& ) const {
			return sigma;
		}

		Value diffusionD( const Value& ) const {
			return 0.0;
		}

		RV<Normal> solution( Value initial, Time t ) const {
			double y = std::exp( -theta * t );
			return initial * y + mu * (1.0-y) + sigma * MakeRV( Normal( 0, (1.0-y*y)/(2.0*theta) ) );
		}

		RV<Normal> solutionLimit() const {
			return mu + sigma * MakeRV( Normal( 0, 0.5/theta ) );
		}

		double expectation( Value initial, Time t ) const {
			double y = std::exp( -theta * t );
			return initial * y + mu * (1.0-y);
		}

		double covariance( Time s, Time t ) const {
			double twotheta = 2.0 * theta;
			return sigma*sigma * std::exp(-theta*(s+t)) * ( std::exp(twotheta * std::fmin(s,t) ) - 1.0 ) / twotheta;
		}

		double variance( Time t ) const {
			double twotheta = 2.0 * theta;
			return sigma*sigma *  ( 1.0 - std::exp(-twotheta*t) ) / twotheta;
		}

	};
		
}
}

#endif
