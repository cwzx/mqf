#ifndef INCLUDED_MQF_TRADING_MOMENTUM
#define INCLUDED_MQF_TRADING_MOMENTUM
#include <vector>

namespace mqf {

	

	template<typename It>
	std::vector<double> logPrices( It p1, It p2 ) {
		auto count = std::distance(p1,p2);
		std::vector<double> results;
		results.reserve(count);
		for(;p1!=p2;++p1) {
			results.push_back( std::log( *p1 ) );
		}
		return results;
	}

}

#endif
