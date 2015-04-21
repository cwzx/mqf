#ifndef INCLUDED_MQF_MOVING_AVERAGE
#define INCLUDED_MQF_MOVING_AVERAGE
#include <vector>
#include <mqf/stats/descriptive.h>

namespace mqf {

	struct SimpleMovingAverage {
		int period;

		explicit SimpleMovingAverage( int period = 2 ) : period(period) {}

		template<typename It>
		std::vector<double> compute( It p1, It p2 ) const {
			int count = (int)std::distance(p1,p2);
			int N = (count + 1) - period;
			if( N < 1 )
				return {};

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
		std::vector<double> compute( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);
			size_t N = (count + 1) - period;
			if( N < 1 )
				return {};

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
