#ifndef INCLUDED_MQF_SMA
#define INCLUDED_MQF_SMA
#include <vector>
#include "../stats/descriptive.h"

namespace mqf {

	struct SMA {
		int period;

		explicit SMA( int period = 2 ) : period(period) {}

		template<typename It>
		auto back( It p1, It p2 ) const {
			using dt = std::iterator_traits<It>::difference_type;
			auto N = std::min<dt>( period, std::distance(p1,p2) );
			if( N < 1 ) return 0.0;
			return sampleMean( std::prev(p2,N), p2 );
		}

		template<typename It>
		std::vector<double> compute( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);
			if( count < period )
				return {};

			size_t N = (count + 1) - period;

			std::vector<double> sma(N);

			It a = p1;
			It b = std::next(p1,period);
			sma[0] = sum( a, b );
			for(size_t i=1;i<N;++i) {
				sma[i] = sma[i-1] - *a + *b;
				++a; ++b;
			}
			for(size_t i=0;i<N;++i) {
				sma[i] /= period;
			}
			return sma;
		}
	};

}

#endif
