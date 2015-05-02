#ifndef INCLUDED_MQF_STATS_EMPIRICAL
#define INCLUDED_MQF_STATS_EMPIRICAL
#include <vector>
#include <algorithm>
#include "descriptive.h"

namespace mqf {
namespace Distributions {

	/*
	 * Empirical Distribution
	 *
	 * Defined by a set of (real) data points.
	 *
	 * The density is not well-defined.
	 *
	 */
	struct Empirical {
		std::vector<double> points;

		Empirical( std::initializer_list<double> ps ) : points(ps) {}

		explicit Empirical( const std::vector<double>& ps ) : points(ps) {}

		explicit Empirical( std::vector<double>&& ps ) : points(std::move(ps)) {}
		
		/*
		 * Empirical cumulative distribution function
		 *
		 */
		double cumulative( double x ) const {
			auto total = points.size();
			auto count = std::count_if( points.begin(), points.end(), [x](double p){ return p <= x; } );
			return (double)count / total;
		}

		/*
		 * Empirical measure for closed intervals [a,b]
		 *
		 */
		double measure( double a, double b ) const {
			auto total = points.size();
			auto count = std::count_if( points.begin(), points.end(), [=](double p){ return a <= p && p <= b; } );
			return (double)count / total;
		}

		double mean() const {
			return sampleMean( points.begin(), points.end() );
		}

		double variance() const {
			return biasedSampleVariance( points.begin(), points.end() );
		}

	};

}
}

#endif
