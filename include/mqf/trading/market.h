#ifndef INCLUDED_MQF_MARKET
#define INCLUDED_MQF_MARKET
#include "portfolio.h"

namespace mqf {

	struct Market {

		std::vector<double> prices;

		double value( const Portfolio& P ) const {
			double sum = 0.0;
			for(size_t i=0;i<P.positions.size();++i) {
				sum += P.positions.values[i] * prices[ P.positions.keys[i] ];
			}
			return sum + P.cash;
		}

	};

}

#endif
