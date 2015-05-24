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
		 * Empirical measure for closed intervals [a,b]
		 *
		 */
		double measure( double a, double b ) const {
			auto total = points.size();
			auto count = std::count_if( points.begin(), points.end(), [=](double p){ return a <= p && p <= b; } );
			return (double)count / total;
		}
		
		double cumulative( double x ) const {
			auto total = points.size();
			auto count = std::count_if( points.begin(), points.end(), [x](double p){ return p <= x; } );
			return (double)count / total;
		}

		double mean() const {
			return sampleMean( points.begin(), points.end() );
		}
		
		double median() const {
			auto N = points.size();
			if( N == 0 ) return 0.0;
			if( N == 1 ) return points[0];

			std::sort( points.begin(), points.end() );

			auto p1 = std::next( points.begin(), N/2 - 1 );
			auto p2 = std::next(p1);
			return secant( *p1, *p2, cumulative(*p1), cumulative(*p2) );
		}

		double variance() const {
			return biasedSampleVariance( points.begin(), points.end() );
		}

		static double secant( double a, double b, double ca, double cb, double t = 0.5 ) {
			return a + (b - a) * (t - ca) / (cb - ca);
		}

	};

}
}

#endif
