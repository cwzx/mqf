#ifndef INCLUDED_MQF_INTEREST
#define INCLUDED_MQF_INTEREST
#include <cmath>
#include "../chrono.h"

namespace mqf {

	struct PeriodInterestRate;
	
	struct InterestRate {
		double rate;

		explicit InterestRate( double rate = 0.0 ) : rate(rate) {}

		double accumulationFactor( Time dt ) const {
			return std::exp( rate * dt );
		}

		double discountFactor( Time dt ) const {
			return std::exp( -rate * dt );
		}

		PeriodInterestRate convertToPeriod( Time newPeriod ) const;

	};

	struct PeriodInterestRate {
		double rate;
		Time period;

		explicit PeriodInterestRate( double rate = 0.0, Time period = 1 ) : rate(rate), period(period) {}

		double accumulationFactor( Time dt ) const {
			return std::pow( 1.0 + rate * period, dt / period );
		}

		double discountFactor( Time dt ) const {
			return 1.0 / accumulationFactor( dt );
		}

		PeriodInterestRate convertToPeriod( Time newPeriod ) const {
			return PeriodInterestRate(
				( std::pow( 1.0 + period * rate, newPeriod / period ) - 1.0 ) / newPeriod,
				newPeriod );
		}

		InterestRate convertToContinuous() const {
			return InterestRate( std::log( 1.0 + period * rate ) / period );
		}

	};

	inline PeriodInterestRate InterestRate::convertToPeriod( Time newPeriod ) const {
		return PeriodInterestRate(
			( std::exp( rate * newPeriod ) - 1.0 ) / newPeriod,
			newPeriod );
	}

	inline double CAGR( double x1, double x2, Time dt ) {
		return std::pow( x2/x1, 1.0/dt ) - 1.0;
	}

}

#endif
