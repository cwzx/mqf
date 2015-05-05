#ifndef INCLUDED_MQF_MARKET
#define INCLUDED_MQF_MARKET
#include "portfolio.h"
#include "../finance/interest.h"

namespace mqf {

	/*
	 * Market
	 *
	 * A set of securities and a riskFreeRate.
	 *
	 */
	struct Market {

		std::vector<double> prices;

		double riskFreeRate = 0.0;

		Gregorian::Date date;

		double value( const Portfolio& P ) const {
			double sum = 0.0;
			for(size_t i=0;i<P.positions.size();++i) {
				sum += P.positions.values[i] * prices[ P.positions.keys[i] ];
			}
			return sum + P.cash * InterestRate(riskFreeRate).accumulationFactor( yearFraction( P.date, date ) );
		}


	};

}

#endif
