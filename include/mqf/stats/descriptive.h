#ifndef INCLUDED_MQF_STATS_DESCRIPTIVE
#define INCLUDED_MQF_STATS_DESCRIPTIVE
#include <limits>
#include <algorithm>
#include <numeric>

namespace mqf {

	template<typename It>
	double sum( It p1, It p2 ) {
		return std::accumulate( p1, p2, 0.0 );
	}

	template<typename It>
	double product( It p1, It p2 ) {
		return std::accumulate( p1, p2, 1.0, std::multiplies<>() );
	}

	template<typename It>
	double sampleMean( It p1, It p2 ) {
		return sum(p1,p2) / std::distance(p1,p2);
	}

	template<typename It>
	double geometricMean( It p1, It p2 ) {
		return std::pow( product(p1,p2), 1.0 / std::distance(p1,p2) );
	}

	template<typename It>
	double harmonicMean( It p1, It p2 ) {
		auto recip = []( double total, double x ) {
			return total + 1.0 / x;
		}
		return double(std::distance(p1,p2)) / std::accumulate( p1, p2, 0.0, recip );
	}

	template<typename It>
	auto min( It p1, It p2 ) {
		return *std::min_element(p1,p2);
	}

	template<typename It>
	auto max( It p1, It p2 ) {
		return *std::max_element(p1,p2);
	}

	template<typename It>
	auto range( It p1, It p2 ) {
		auto mm = std::minmax(p1,p2);
		return mm.second - mm.first;
	}

	template<typename It>
	double biasedSampleVariance( It p1, It p2 ) {
		auto mean = sampleMean(p1,p2);
		auto squares = [mean]( double total, double x ) {
			double r = x - mean;
			return total + r*r;
		}
		return std::accumulate(p1,p2,0.0,squares) / std::distance(p1,p2);
	}

	template<typename It>
	double unbiasedSampleVariance( It p1, It p2 ) {
		auto mean = sampleMean(p1,p2);
		auto squares = [mean]( double total, double x ) {
			double r = x - mean;
			return total + r*r;
		}
		return std::accumulate(p1,p2,0.0,squares) / ( std::distance(p1,p2) - 1 );
	}

	inline double Return( double x0, double x1 ) {
		return (x1 - x0) / x0;
	}

	inline double logReturn( double x0, double x1 ) {
		return std::log( x1 / x0 );
	}

	template<typename It>
	double averageReturn( It p1, It p2 ) {
		double sum = 0.0;
		size_t count = 0;
		double current = *p1;
		while( p1 != p2 ) {
			double previous = current;
			current = *++p1;
			sum += Return( previous, current );
			++count;
		}
		return sum / count;
	}

	template<typename It>
	double averageLogReturn( It p1, It p2 ) {
		double sum = 0.0;
		size_t count = 0;
		double current = *p1;
		while( p1 != p2 ) {
			double previous = current;
			current = *++p1;
			sum += logReturn( previous, current );
			++count;
		}
		return sum / count; 
	}



}

#endif
