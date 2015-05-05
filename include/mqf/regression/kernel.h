#ifndef INCLUDED_MQF_REGRESSION_KERNEL
#define INCLUDED_MQF_REGRESSION_KERNEL
#include <vector>
#include <numeric>

namespace mqf {

	template<typename Kernel>
	struct KernelRegression {
		Kernel kernel;
		std::vector<double> X, Y;
		double rbw = 1.0;

		double operator()( double x ) const {
			auto xp = X.begin();
			auto xe = X.end();
			auto yp = Y.begin();
			auto ye = Y.end();
			double sum1 = 0.0, sum2 = 0.0;
			for(;xp!=xe;++xp,++yp) {
				auto k = kernel( (x - *xp) * rbw ) * rbw;
				sum1 += k * *yp;
				sum2 += k;
			}
			return sum1 / sum2;
		}

		void setBandwidth( double bw ) {
			rbw = 1.0 / bw;
		}

	};

}

#endif
