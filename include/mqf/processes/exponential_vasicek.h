#ifndef INCLUDED_MQF_PROCESSES_EXPONENTIAL_VASICEK
#define INCLUDED_MQF_PROCESSES_EXPONENTIAL_VASICEK
#include "../process.h"
#include "../distributions/log_normal.h"

namespace mqf {
namespace Processes {

	/*
	 * Exponential Vasicek
	 *
	 * d log(X_t) = k ( theta - log(X_t) ) dt + sigma dW_t
	 *
	 */
	struct ExponentialVasicek : Process<double> {
		using Normal    = Distributions::Normal;
		using LogNormal = Distributions::LogNormal;

		double k, theta, sigma;

		explicit ExponentialVasicek( double mean = 1.0,
			                         double reversion = 1.0,
			                         double volatility = 1.0 ) :
			theta(mean),
			k(reversion),
			sigma(volatility)
		{}

		Value drift( const Value& X ) const {
			return ( k * ( theta - std::log(X) ) + 0.5*sigma*sigma ) * X;
		}

		Value driftD( const Value& X ) const {
			return k * ( theta - 1.0 - std::log(X) ) + 0.5*sigma*sigma;
		}

		Value diffusion( const Value& X ) const {
			return sigma * X;
		}

		Value diffusionD( const Value& ) const {
			return sigma;
		}

		RV<LogNormal> solution( Value initial, Time t ) const {
			double y = std::exp( -k * t );
			return exp( std::log(initial) * y
			          + theta * (1.0-y)
			          + sigma * MakeRV( Normal( 0, (1.0-y*y)/(2.0*k) ) ) );
		}

		double expectation( Value initial, Time t ) const {
			double y = std::exp( -k * t );
			return std::exp( std::log(initial) * y
			               + theta * (1.0-y)
			               + sigma*sigma * (1.0-y*y) / (4.0*k) );
		}

		double variance( Value initial, Time t ) const {
			double E = expectation( initial, t );
			double y = std::exp( -k * t );
			return E*E * ( std::exp( sigma*sigma * ( 1.0 - y*y ) / (2.0*k) ) - 1.0 );
		}

		RV<LogNormal> solutionLimit() const {
			return exp( theta + sigma * MakeRV( Normal( 0, 0.5/k ) ) );
		}

		double expectationLimit() const {
			return std::exp( theta + sigma*sigma / (4.0*k) );
		}

		double varianceLimit() const {
			double y = std::exp( sigma*sigma / (2.0*k) );
			return std::exp( 2.0 * theta ) * y * ( y - 1.0 );
		}

	};
		
}
}

#endif
