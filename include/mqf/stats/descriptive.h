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
	double sampleVariance( It p1, It p2 ) {
		auto mean = sampleMean(p1,p2);
		auto squares = [mean]( double total, double x ) {
			double r = x - mean;
			return total + r*r;
		};
		return std::accumulate(p1,p2,0.0,squares) / ( std::distance(p1,p2) - 1 );
	}

	template<typename Itp,typename Itq>
	double sampleCovariance( Itp lhs1, Itp lhs2, Itq rhs1, Itq rhs2 ) {
		auto count = std::min( std::distance(lhs1,lhs2), std::distance(rhs1,rhs2) );
		lhs2 = std::next(lhs1,count);
		rhs2 = std::next(rhs1,count);
		auto lhs_mean = sampleMean(lhs1,lhs2);
		auto rhs_mean = sampleMean(rhs1,rhs2);
		double sum = 0.0;
		for(size_t i=0;i<count;++i) {
			sum += ( *lhs1 - lhs_mean ) * ( *rhs1 - rhs_mean );
			++lhs1; ++rhs1;
		}
		return sum / ( count - 1 );
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
		auto count = std::distance( p1, p2 );
		if( count < 2 ) return 0.0;
		return logReturn( *p1, *--p2 ) / ( count - 1 ); 
	}



}

#endif
