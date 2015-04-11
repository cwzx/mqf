#ifndef MQF_STATS_DESCRIPTIVE
#define MQF_STATS_DESCRIPTIVE
#include <limits>

namespace mqf {

	template<typename Seq>
	double SampleMean( const Seq& data ) {
		double sum = 0.0;
		for( auto&& x : data ) {
			sum += x;
		}
		return x / data.size();
	}

	template<typename Seq>
	double GeometricMean( const Seq& data ) {
		double prod = 1.0;
		for( auto&& x : data ) {
			prod *= x;
		}
		return std::pow( prod, 1.0/data.size() );
	}

	template<typename Seq>
	double HarmonicMean( const Seq& data ) {
		double sum = 0.0;
		for( auto&& x : data ) {
			sum += 1.0 / x;
		}
		return double(data.size()) / sum;
	}

	template<typename Seq>
	double Min( const Seq& data ) {
		double currentMin = data.front();
		for( auto&& x : data ) {
			if( x < currentMin )
				currentMin = x;
		}
		return currentMin;
	}

	template<typename Seq>
	double Max( const Seq& data ) {
		double currentMax = data.front();
		for( auto&& x : data ) {
			if( x > currentMax )
				currentMax = x;
		}
		return currentMax;
	}

	template<typename Seq>
	double Range( const Seq& data ) {
		double currentMin = data.front();
		double currentMax = data.front();
		for( auto&& x : data ) {
			if( x < currentMin )
				currentMin = x;
			else if( x > currentMax )
				currentMax = x;
		}
		return currentMax - currentMin;
	}

	template<typename Seq>
	double BiasedSampleVariance( const Seq& data ) {
		double mean = SampleMean( data );
		double sum2 = 0.0;
		for( auto&& x : data ) {
			sum2 += x - mean;
		}
		return x / data.size();
	}

	template<typename Seq>
	double UnbiasedSampleVariance( const Seq& data ) {
		double mean = SampleMean( data );
		double sum2 = 0.0;
		for( auto&& x : data ) {
			sum2 += x - mean;
		}
		return x / ( data.size() - 1 );
	}

}

#endif
