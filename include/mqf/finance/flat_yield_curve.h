#ifndef INCLUDED_MQF_FLAT_YIELD_CURVE
#define INCLUDED_MQF_FLAT_YIELD_CURVE
#include "../chrono.h"
#include <cmath>

namespace mqf {

	struct FlatYieldCurve {
		double rate;

		explicit FlatYieldCurve( double rate = 0.0 ) : rate(rate) {}
		
		double zcb( double T ) const {
			return std::exp( -rate * T );
		}

		double simpleSpotRate( double T ) const {
			auto P = zcb(T);
			return ( 1.0 - P ) / ( T * P );
		}

		// spot rate
		double operator()( double ) const {
			return rate;
		}

		double shortRate() const {
			return rate;
		}

		double simpleForwardRate( double T, double S ) const {
			return ( std::exp( -rate * (S - T) ) - 1.0 ) / (S - T);
		}

		double forwardRate( double, double ) const {
			return rate;
		}

		double instantaneousForwardRate( double ) const {
			return rate;
		}

	};

}

#endif
