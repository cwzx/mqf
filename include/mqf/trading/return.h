#ifndef INCLUDED_MQF_TRADING_RETURN
#define INCLUDED_MQF_TRADING_RETURN
#include <cmath>
#include <vector>

namespace mqf {

	inline double Return( double x0, double x1 ) {
		return (x1 - x0) / x0;
	}

	inline double logReturn( double x0, double x1 ) {
		return std::log( x1 / x0 );
	}

	inline double returnFromLogReturn( double logReturn ) {
		return std::exp( logReturn ) - 1.0;
	}

	template<typename It>
	double cumulativeLogReturn( It p1, It p2 ) {
		return logReturn( *p1, *--p2 ); 
	}

	template<typename It>
	double cumulativeReturn( It p1, It p2 ) {
		return returnFromLogReturn( cumulativeLogReturn( p1, p2 ) );
	}

	template<typename It>
	double meanReturn( It p1, It p2 ) {
		auto count = std::distance( p1, p2 );
		if( count < 2 ) return 0.0;
		double sum = 0.0;
		for( auto q = p1++; p1 != p2; ++p1,++q ) {
			sum += Return(*q,*p1);
		}
		return sum / (count - 1);
	}

	template<typename It>
	double meanLogReturn( It p1, It p2 ) {
		auto count = std::distance( p1, p2 );
		if( count < 2 ) return 0.0;
		return cumulativeLogReturn( p1, p2 ) / (count - 1);
	}

	template<typename It>
	std::vector<double> computeReturns( It p1, It p2 ) {
		std::vector<double> ret;
		auto count = std::distance( p1, p2 );
		if( count < 2 ) return ret;
		ret.reserve( count - 1 );
		for( auto q = p1++; p1 != p2; ++p1,++q ) {
			ret.push_back( Return(*q,*p1) );
		}
		return ret;
	}

	template<typename It>
	std::vector<double> computeLogReturns( It p1, It p2 ) {
		std::vector<double> ret;
		auto count = std::distance( p1, p2 );
		if( count < 2 ) return ret;
		ret.reserve( count - 1 );
		for( auto q = p1++; p1 != p2; ++p1,++q ) {
			ret.push_back( logReturn(*q,*p1) );
		}
		return ret;
	}

	template<typename It>
	std::vector<double> computeCumulativeReturns( It p1, It p2 ) {
		std::vector<double> ret;
		auto count = std::distance( p1, p2 );
		if( count < 2 ) return ret;
		ret.reserve( count - 1 );
		auto initial = *p1;
		for(;p1!=p2;++p1) {
			ret.push_back( Return( initial, *p1 ) );
		}
		return ret;
	}

	template<typename It>
	std::vector<double> computeCumulativeLogReturns( It p1, It p2 ) {
		std::vector<double> ret;
		auto count = std::distance( p1, p2 );
		if( count < 2 ) return ret;
		ret.reserve( count - 1 );
		auto initial = *p1;
		for(;p1!=p2;++p1) {
			ret.push_back( logReturn( initial, *p1 ) );
		}
		return ret;
	}

	template<typename It>
	double downsideVariance( It p1, It p2, double target ) {
		auto N = std::distance(p1,p2);
		double dv = 0.0;
		for(;p1!=p2;++p1) {
			auto d = *p1 - target;
			if( !( d < 0.0 ) )
				continue;
			dv += d * d;
		}
		return dv / N;
	}

	template<typename It>
	double downsideRisk( It p1, It p2, double target ) {
		return std::sqrt( downsideVariance(p1,p2,target) );
	}

	template<typename It>
	double shortfallRisk( It p1, It p2, double target ) {
		auto N = std::distance(p1,p2);
		return std::count_if( p1, p2, [=]( double x ){ return x < target; } ) / N;
	}

}

#endif
