#ifndef INCLUDED_MQF_STATS_MVUE
#define INCLUDED_MQF_STATS_MVUE
#include "../distributions/normal.h"
#include "descriptive.h"

namespace mqf {

	template<typename>
	struct MVUE;

	template<>
	struct MVUE<Distributions::Normal> {
		using Normal = Distributions::Normal;
		template<typename It>
		Normal operator()( It p1, It p2 ) const {
			return Normal( sampleMean(p1,p2), sampleVariance(p1,p2) );
		}
	};

}

#endif
