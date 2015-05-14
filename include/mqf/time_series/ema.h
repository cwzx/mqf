#ifndef INCLUDED_MQF_EMA
#define INCLUDED_MQF_EMA
#include <vector>

namespace mqf {

	struct EMA {
		double alpha;

		explicit EMA( double alpha = 0.1 ) : alpha(alpha) {}
		
		double update( double ema, double x ) const {
			return (1.0-alpha) * ema + alpha * x;
		}

		template<typename It>
		auto back( It p1, It p2 ) const {
			auto ema = *p1;
			for(++p1;p1!=p2;++p1) {
				ema += alpha * (*p1 - ema);
			}
			return ema;
		}

		template<typename It>
		std::vector<double> compute( It p1, It p2 ) const {
			auto N = std::distance(p1,p2);
			std::vector<double> ema(N);
			ema[0] = *p1;
			for(size_t i=1;i<N;++i) {
				ema[i] = (1.0-alpha) * ema[i-1] + alpha * (*++p1);
			}
			return ema;
		}
	};
}

#endif
