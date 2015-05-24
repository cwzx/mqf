#ifndef INCLUDED_MQF_STATS_DESCRIPTIVE
#define INCLUDED_MQF_STATS_DESCRIPTIVE
#include <limits>
#include <algorithm>
#include <numeric>
#include "../utility.h"

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
	double logGeometricMean( It p1, It p2 ) {
		return std::log( product(p1,p2) ) / std::distance(p1,p2);
	}

	template<typename It>
	double meanLog( It p1, It p2 ) {
		return logGeometricMean(p1,p2);
	}

	template<typename It>
	double harmonicMean( It p1, It p2 ) {
		auto recip = []( double total, double x ) {
			return total + 1.0 / x;
		};
		return (double)std::distance(p1,p2) / std::accumulate( p1, p2, 0.0, recip );
	}

	template<typename It>
	double logHarmonicMean( It p1, It p2 ) {
		auto recip = []( double total, double x ) {
			return total + 1.0 / x;
		};
		return -std::log( std::accumulate( p1, p2, 0.0, recip ) / std::distance(p1,p2) );
	}

	template<typename It>
	double meanReciprocal( It p1, It p2 ) {
		auto recip = []( double total, double x ) {
			return total + 1.0 / x;
		};
		return std::accumulate(p1,p2,0.0,recip) / std::distance(p1,p2);
	}

	template<typename It>
	double meanSquare( It p1, It p2 ) {
		auto squares = []( double total, double x ) {
			return total + x*x;
		};
		return std::accumulate(p1,p2,0.0,squares) / std::distance(p1,p2);
	}

	template<typename It>
	double rootMeanSquare( It p1, It p2 ) {
		return std::sqrt( meanSquare(p1,p2) );
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
	double biasedSampleVariance( It p1, It p2, double mean ) {
		auto squares = [mean]( double total, double x ) {
			return total + square(x - mean);
		};
		return std::accumulate(p1,p2,0.0,squares) / std::distance(p1,p2);
	}

	template<typename It>
	double biasedSampleVariance( It p1, It p2 ) {
		return biasedSampleVariance(p1,p2,sampleMean(p1,p2));
	}

	template<typename It>
	double sampleVariance( It p1, It p2, double mean ) {
		auto squares = [mean]( double total, double x ) {
			return total + square(x - mean);
		};
		return std::accumulate(p1,p2,0.0,squares) / ( std::distance(p1,p2) - 1 );
	}

	template<typename It>
	double sampleVariance( It p1, It p2 ) {
		return sampleVariance( p1, p2, sampleMean(p1,p2) );
	}

	template<typename It>
	double sampleStdDev( It p1, It p2 ) {
		return std::sqrt( sampleVariance( p1, p2 ) );
	}
	
	template<typename It>
	double sampleStdDev( It p1, It p2, double mean ) {
		return std::sqrt( sampleVariance( p1, p2, mean ) );
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

}

#endif
