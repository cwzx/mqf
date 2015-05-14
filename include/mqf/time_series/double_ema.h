#ifndef INCLUDED_MQF_DOUBLE_EMA
#define INCLUDED_MQF_DOUBLE_EMA
#include <vector>

namespace mqf {

	/*
	 * Double Exponential Moving Average
	 *
	 * EMA( EMA( x, alpha ), beta )
	 *
	 * Symmetric in (alpha,beta)
	 *
	 */
	struct DoubleEMA {
		double alpha, beta;

		explicit DoubleEMA( double alpha = 0.1, double beta = 0.1 ) : alpha(alpha), beta(beta) {}

		template<typename It>
		auto back( It p1, It p2 ) const {
			auto dema = *p1;
			auto prev_dema = dema;
			auto c1 = 2.0 - alpha - beta;
			auto c2 = (1.0 - alpha) * (1.0 - beta);
			auto c3 = alpha * beta;
			for(++p1;p1!=p2;++p1) {
				auto next_dema = c1 * dema - c2 * prev_dema + c3 * *p1;
				prev_dema = dema;
				dema = next_dema;
			}
			return dema;
		}

		template<typename It>
		std::vector<double> compute( It p1, It p2 ) const {
			std::vector<double> vdema( std::distance(p1,p2) );
			auto q = vdema.begin();
			auto dema = *p1;
			auto prev_dema = dema;
			*q = dema;
			auto c1 = 2.0 - alpha - beta;
			auto c2 = (1.0 - alpha) * (1.0 - beta);
			auto c3 = alpha * beta;
			for(++p1,++q;p1!=p2;++p1,++q) {
				auto next_dema = c1 * dema - c2 * prev_dema + c3 * *p1;
				prev_dema = dema;
				dema = next_dema;
				*q = dema;
			}
			return vdema;
		}
	};
}

#endif
