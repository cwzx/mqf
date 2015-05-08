#ifndef INCLUDED_MQF_TIME_SERIES_ARMA
#define INCLUDED_MQF_TIME_SERIES_ARMA
#include <cstdint>
#include "../regression/least_squares.h"

namespace mqf {
namespace TimeSeries {

	template<typename T,uint32_t P>
	struct AR {
		using Vec = Eigen::Matrix<T,P+1,1>;
		
		Vec coefficients;

		template<typename It>
		void compute( It p1, It p2 ) {
			auto N = std::distance(p1,p2);
			auto M = N - P;

			Eigen::Matrix<T,-1,1> data(N);
			for(size_t i=0;i<N;++i) {
				data[i] = *p1;
				++p1;
			}

			Eigen::Matrix<T,-1,P+1> X(M);
			X.col(0).setOnes();
			for(size_t j=0;j<P;++j)
				X.col(P-j) = data.segment(j,M);

			coefficients = OLS(data.tail<M>(),X);
		}

	};




}
}

#endif
