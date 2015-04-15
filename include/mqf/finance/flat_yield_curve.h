#ifndef INCLUDED_MQF_FLAT_YIELD_CURVE
#define INCLUDED_MQF_FLAT_YIELD_CURVE
#include "../chrono.h"
#include <cmath>

namespace mqf {

	struct FlatYieldCurve {
		double rate;

		explicit FlatYieldCurve( double rate = 0.0 ) : rate(rate) {}
		
		double zcb( Time T ) const {
			return std::exp( -rate * T );
		}

		double simpleSpotRate( Time T ) const {
			auto P = zcb(T);
			return ( 1.0 - P ) / ( T * P );
		}

		// spot rate
		double operator()( Time ) const {
			return rate;
		}

		double shortRate() const {
			return rate;
		}

		double simpleForwardRate( Time T, Time S ) const {
			return ( std::exp( -rate * (S - T) ) - 1.0 ) / (S - T);
		}

		double forwardRate( Time, Time ) const {
			return rate;
		}

		double instantaneousForwardRate( Time ) const {
			return rate;
		}

	};

}

#endif
