#ifndef INCLUDED_MQF_INTEREST
#define INCLUDED_MQF_INTEREST
#include <cmath>
#include "../chrono.h"

namespace mqf {

	struct PeriodInterestRate;
	
	struct InterestRate {
		double rate;

		explicit InterestRate( double rate = 0.0 ) : rate(rate) {}

		double accumulationFactor( double dt ) const {
			return std::exp( rate * dt );
		}

		double discountFactor( double dt ) const {
			return std::exp( -rate * dt );
		}

		PeriodInterestRate convertToPeriod( double newPeriod ) const;

	};

	struct PeriodInterestRate {
		double rate;
		double period;

		explicit PeriodInterestRate( double rate = 0.0, double period = 1 ) : rate(rate), period(period) {}

		double accumulationFactor( double dt ) const {
			return std::pow( 1.0 + rate * period, dt / period );
		}

		double discountFactor( double dt ) const {
			return 1.0 / accumulationFactor( dt );
		}

		PeriodInterestRate convertToPeriod( double newPeriod ) const {
			return PeriodInterestRate(
				( std::pow( 1.0 + period * rate, newPeriod / period ) - 1.0 ) / newPeriod,
				newPeriod );
		}

		InterestRate convertToContinuous() const {
			return InterestRate( std::log( 1.0 + period * rate ) / period );
		}

	};

	inline PeriodInterestRate InterestRate::convertToPeriod( double newPeriod ) const {
		return PeriodInterestRate(
			( std::exp( rate * newPeriod ) - 1.0 ) / newPeriod,
			newPeriod );
	}

	inline double CAGR( double x1, double x2, double dt ) {
		return std::pow( x2/x1, 1.0/dt ) - 1.0;
	}

}

#endif
