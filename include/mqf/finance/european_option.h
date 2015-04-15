#ifndef INCLUDED_MQF_EUROPEAN_OPTION
#define INCLUDED_MQF_EUROPEAN_OPTION
#include <cmath>
#include "../chrono.h"
#include "../distributions/normal.h"

namespace mqf {
namespace Options {

	struct VanillaCallPayoff {
		double strike;

		explicit VanillaCallPayoff( double strike ) : strike(strike) {}

		double operator()( double underlying ) const {
			return std::fmax( underlying - strike, 0 );
		}

	};

	struct VanillaPutPayoff {
		double strike;
		
		explicit VanillaPutPayoff( double strike ) : strike(strike) {}

		double operator()( double underlying ) const {
			return std::fmax( strike - underlying, 0 );
		}
	};
	
	template<typename Date>
	struct VanillaOption {
		double strike;
		Date expiry;
	};

	struct BlackScholes {
		
		static double PriceCall( double strike,
		                         double underlying,
		                         Time timeToExpiry,
		                         double riskFreeRate,
		                         double volatility,
		                         double dividendRate = 0.0 )
		{
			Distributions::StdNormal N;
			double denom = volatility * std::sqrt(timeToExpiry);
			double halfSigma2Tau = 0.5 * volatility * volatility * timeToExpiry;
			double forward = std::exp( (riskFreeRate - dividendRate) * timeToExpiry ) * underlying;
			double logRatio = std::log( forward / strike );
			double d1 = ( logRatio - halfSigma2Tau ) / denom;
			double d2 = ( logRatio + halfSigma2Tau ) / denom;
			double N1 = N.cumulative( d1 );
			double N2 = N.cumulative( d2 );
			double discount = std::exp( -riskFreeRate * timeToExpiry );
			return discount * ( forward * N1 - strike * N2 );
		}

		static double PricePut( double strike,
		                        double underlying,
		                        Time timeToExpiry,
		                        double riskFreeRate,
		                        double volatility,
		                        double dividendRate = 0.0 )
		{
			Distributions::StdNormal N;
			double denom = volatility * std::sqrt(timeToExpiry);
			double halfSigma2Tau = 0.5 * volatility * volatility * timeToExpiry;
			double forward = std::exp( (riskFreeRate - dividendRate) * timeToExpiry ) * underlying;
			double logRatio = std::log( forward / strike );
			double d1 = ( logRatio - halfSigma2Tau ) / denom;
			double d2 = ( logRatio + halfSigma2Tau ) / denom;
			double N1 = N.cumulative( -d1 );
			double N2 = N.cumulative( -d2 );
			double discount = std::exp( -riskFreeRate * timeToExpiry );
			return discount * ( strike * N2 - forward * N1 );
		}

	};

}
}

#endif
