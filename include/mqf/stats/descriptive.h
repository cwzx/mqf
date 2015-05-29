#ifndef INCLUDED_MQF_STATS_DESCRIPTIVE
#define INCLUDED_MQF_STATS_DESCRIPTIVE
#include <limits>
#include <algorithm>
#include <numeric>
#include <tuple>
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
		auto mm = std::minmax_element(p1,p2);
		return *mm.second - *mm.first;
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
	double sampleStdDev( It p1, It p2, double mean ) {
		return std::sqrt( sampleVariance( p1, p2, mean ) );
	}

	template<typename It>
	double sampleStdDev( It p1, It p2 ) {
		return sampleStdDev( p1, p2, sampleMean(p1,p2) );
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

	template<typename Itp,typename Itq>
	double sampleCorrelation( Itp lhs1, Itp lhs2, Itq rhs1, Itq rhs2 ) {
		auto count = std::min( std::distance(lhs1,lhs2), std::distance(rhs1,rhs2) );
		lhs2 = std::next(lhs1,count);
		rhs2 = std::next(rhs1,count);
		auto lhs_mean = sampleMean(lhs1,lhs2);
		auto rhs_mean = sampleMean(rhs1,rhs2);
		double sumXY = 0.0, sumXX = 0.0, sumYY = 0.0;
		for(size_t i=0;i<count;++i) {
			auto x = *lhs1 - lhs_mean;
			auto y = *rhs1 - rhs_mean;
			sumXY += x*y;
			sumXX += x*x;
			sumYY += y*y;
			++lhs1; ++rhs1;
		}
		return sumXY / std::sqrt( sumXX * sumYY );
	}

	template<typename It>
	double sampleSkewness( It p1, It p2, double mean ) {
		auto sqcu = [mean]( auto total, double x ) {
			auto y = x - mean;
			return make_pair( total.first + square(y), total.second + cube(y) );
		};
		auto x = std::accumulate( p1, p2, make_pair(0.0,0.0), sqcu );
		auto N = std::distance(p1,p2);
		return x.second * std::sqrt( (N - 1) / cube( x.first ) ) * N / (N - 2);
	}

	template<typename It>
	double sampleSkewness( It p1, It p2 ) {
		return sampleSkewness( p1, p2, sampleMean(p1,p2) );
	}

	template<typename It>
	double sampleExKurtosis( It p1, It p2, double mean ) {
		auto sqqu = [mean]( auto total, double x ) {
			auto y2 = square(x - mean);
			return make_pair( total.first + y2, total.second + y2 * y2 );
		};
		auto x = std::accumulate( p1, p2, make_pair(0.0,0.0), sqqu );
		auto N = std::distance(p1,p2);
		return ( ( N * (N + 1) * x.second / square(x.first) - 3.0 * (N - 1) ) * (N - 1) )
		     / ( (N - 2) * (N - 3) );
	}

	template<typename It>
	double sampleExKurtosis( It p1, It p2 ) {
		return sampleExKurtosis( p1, p2, sampleMean(p1,p2) );
	}

}

#endif
