#ifndef INCLUDED_MQF_INTEREST
#define INCLUDED_MQF_INTEREST
#include <cmath>
#include "../chrono.h"

namespace mqf {

	enum Compounding {
		Period,
		Continuous
	};

	template<Compounding = Continuous>
	struct InterestRate;

	template<>
	struct InterestRate<Continuous> {
		double rate;

		explicit InterestRate( double rate = 0.0 ) : rate(rate) {}

		double accumulationFactor( Time dt ) const {
			return std::exp( rate * dt );
		}

		double discountFactor( Time dt ) const {
			return std::exp( -rate * dt );
		}

		InterestRate<Period> convertToPeriod( Time newPeriod ) const {
			return InterestRate<Period>(
				( std::exp( rate * newPeriod ) - 1.0 ) / newPeriod,
				newPeriod );
		}

	};

	template<>
	struct InterestRate<Period> {
		double rate;
		Time period;

		explicit InterestRate( double rate = 0.0, Time period = 1 ) : rate(rate), period(period) {}

		double accumulationFactor( Time dt ) const {
			return std::pow( 1.0 + rate * period, dt / period );
		}

		double discountFactor( Time dt ) const {
			return 1.0 / accumulationFactor( dt );
		}

		InterestRate<Period> convertToPeriod( Time newPeriod ) const {
			return InterestRate<Period>(
				( std::pow( 1.0 + period * rate, newPeriod / period ) - 1.0 ) / newPeriod,
				newPeriod );
		}

		InterestRate<Continuous> convertToContinuous() const {
			return InterestRate<Continuous>( std::log( 1.0 + period * rate ) / period );
		}

	};

	double CAGR( double x1, double x2, Time dt ) {
		return std::pow( x2/x1, 1.0/dt ) - 1.0;
	}

}

#endif
