#ifndef INCLUDED_MQF_STATS_KDE
#define INCLUDED_MQF_STATS_KDE
#include <vector>
#include <numeric>
#include "descriptive.h"

namespace mqf {

	template<typename Kernel>
	struct KernelDensityEstimator {
		Kernel kernel;
		std::vector<double> points;
		double rbw = 1.0;

		double operator()( double x ) const {
			auto f = [&,x,rbw]( double total, double p ) {
				return total + kernel( (x - p) * rbw ) * rbw;
			};
			return std::accumulate( points.begin(), points.end(), f ) / points.size();
		}

		void setBandwidth( double bw ) {
			rbw = 1.0 / bw;
		}

		void setGaussianBandwidth() {
			auto n = points.size();
			auto stddev = std::sqrt( sampleVariance( points.begin(), points.end() ) );
			rbw = std::pow( 0.75 * n, 0.2 ) / stddev;
		}

	};

}

#endif
