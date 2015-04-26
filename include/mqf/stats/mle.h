#ifndef INCLUDED_MQF_MLE
#define INCLUDED_MQF_MLE
#include "../distributions/normal.h"
#include "descriptive.h"

namespace mqf {
	
	template<typename>
	struct MLE;

	template<>
	struct MLE<Distributions::Normal> {
		using Normal = Distributions::Normal;
		template<typename It>
		Normal operator()( It p1, It p2 ) const {
			return Normal( sampleMean(p1,p2), biasedSampleVariance(p1,p2) );
		}
	};

}

#endif
