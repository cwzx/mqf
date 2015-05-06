#ifndef INCLUDED_MQF_WMA
#define INCLUDED_MQF_WMA
#include <vector>
#include "../stats/descriptive.h"

namespace mqf {

	struct WeightedMovingAverage {
		int period;

		explicit WeightedMovingAverage( int period = 2 ) : period(period) {}

		template<typename It>
		static double weightedSum( It p1, It p2 ) {
			size_t i = 1;
			double sum = 0.0;
			for(; p1 != p2; ++p1) {
				sum += *p1 * i;
				++i;
			}
			return sum;
		}

		template<typename It>
		static double weightedAvg( It p1, It p2 ) {
			auto N = std::distance(p1,p2);
			return ( N > 0 ) ? weightedSum(p1,p2) / ((N*(N+1))/2) : 0.0;
		}

		template<typename It>
		auto back( It p1, It p2 ) const {
			using dt = std::iterator_traits<It>::difference_type;
			auto N = std::min<dt>( period, std::distance(p1,p2) );
			return weightedAvg( std::prev(p2,N), p2 );
		}

		template<typename It>
		std::vector<double> compute( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);
			if( count < period )
				return {};

			size_t N = (count + 1) - period;

			std::vector<double> wma(N);
			
			It a = p1;
			It b = std::next(p1,period);
			double total = sum( a, b );
			wma[0] = weightedSum( a, b );
			for(size_t i=1;i<N;++i) {
				wma[i] = wma[i-1] + *b * period - total;
				total += *b - *a;
				++a; ++b;
			}
			auto denom = (period*(period+1))/2;
			for(size_t i=0;i<N;++i) {
				wma[i] /= denom;
			}
			return wma;
		}
	};

}

#endif
