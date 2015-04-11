#ifndef INCLUDED_MQF_TIME_SERIES_ARMA
#define INCLUDED_MQF_TIME_SERIES_ARMA
#include <cstdint>
#include <vector>

namespace mqf {
namespace TimeSeries {
	
	template<typename T,uint32_t p,uint32_t q>
	struct ARMA {
		T constant;
		T autoregressive[p];
		T moving_average[q];
	};

	template<typename T,uint32_t p>
	struct ARMA<T,p,0> {
		T constant;
		T autoregressive[p];
	};

	template<typename T,uint32_t q>
	struct ARMA<T,0,q> {
		T constant;
		T moving_average[q];
	};

	template<typename T>
	struct ARMA<T,0,0> {
		T constant;
	};


	template<typename T,uint32_t p>
	using AR = ARMA<T,p,0>;

	template<typename T,uint32_t q>
	using MA = ARMA<T,0,q>;

}
}

#endif
