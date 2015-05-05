#ifndef INCLUDED_MQF_STATS_HISTOGRAM
#define INCLUDED_MQF_STATS_HISTOGRAM
#include <cstdint>
#include <vector>
#include <numeric>
#include <fstream>

namespace mqf {

	/*
	 * Histogram
	 *
	 * Bins are defined by a set of N values,
	 * 
	 * -\infty < x_1 < ... < x_N < \infty
	 *
	 * (-\infty,x_1), [x_1,x_2), ..., [x_{N-1},x_N), [x_N,\infty)
	 *
	 */
	struct Histogram {
		std::vector<double> bins;
		std::vector<uint32_t> frequencies;
		
		explicit Histogram( size_t N = 100 ) : bins(N), frequencies(N+1) {}

		uint64_t totalFrequency() const {
			return std::accumulate( frequencies.begin(), frequencies.end(), uint64_t(0) );
		}

		double binWidth( size_t i ) const {
			if(i == 0) {
				return std::numeric_limits<double>::infinity();
			}
			if(i == bins.size()) {
				return std::numeric_limits<double>::infinity();
			}
			return bins[i] - bins[i-1];
		}

		double binCentre( size_t i ) const {
			if(i == 0) {
				return -std::numeric_limits<double>::infinity();
			}
			if(i == bins.size()) {
				return std::numeric_limits<double>::infinity();
			}
			return (bins[i] + bins[i-1]) * 0.5;
		}

		uint32_t binFrequency( size_t i ) const {
			return frequencies[i];
		}

		double binFreqDensity( size_t i ) const {
			return (double)frequencies[i] / binWidth(i);
		}

		void incrementBin( size_t i ) {
			++frequencies[i];
		}

		size_t getBinForValue( double x ) const {
			for(size_t i = 0;i<bins.size();++i) {
				if( x < bins[i] )
					return i;
			}
			return bins.size();
		}

		void writeCSV( const char* filename ) const {
			using namespace std;
			ofstream out(filename);
			if( !out ) return;
			auto total = totalFrequency();
			for(size_t i = 0;i<frequencies.size();++i) {
				double freqDensity = binFreqDensity(i);
				out << binCentre(i)                   << ','
				    << frequencies[i]                 << ','
				    << (double)frequencies[i] / total << ','
				    << freqDensity                    << ','
				    << freqDensity / total            << endl;
			}
		}

	};

	/*
	 * A class for generating histograms from data.
	 */
	struct HistogramGenerator {
		uint32_t numBins;                  // number of bins
		double binMin = 0, binMax = 0;     // manually specify bin min and max values
		bool autoBin = true;               // automatically determine bin sizes

		explicit HistogramGenerator( uint32_t nBins = 0 ) : numBins(nBins) {}

		template<typename It>
		Histogram generate( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);
	
			uint32_t nBins = numBins;
			if( nBins == 0 ) {
				nBins = uint32_t(1 + count / 20);
			}

			auto minVal = binMin;
			auto maxVal = binMax;
			if( autoBin ) {
				auto minmax = std::minmax_element(p1,p2);
				minVal = *minmax.first;
				maxVal = *minmax.second;
			}

			Histogram H = setupBins( minVal, maxVal, nBins );

			for(;p1!=p2;++p1) {
				H.incrementBin( H.getBinForValue( *p1 ) );
			}
			return H;
		}

		template<typename It>
		Histogram generateLog( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);
	
			uint32_t nBins = numBins;
			if( nBins == 0 ) {
				nBins = uint32_t(1 + count / 20);
			}
	
			auto minVal = binMin;
			auto maxVal = binMax;
			if( autoBin ) {
				auto minmax = std::minmax_element(p1,p2);
				minVal = *minmax.first;
				maxVal = *minmax.second;
			}
			
			Histogram H = setupLogBins( minVal, maxVal, nBins );
			
			for(;p1!=p2;++p1) {
				H.incrementBin( H.getBinForValue( *p1 ) );
			}
			return H;
		}

		static Histogram setupBins( double minVal, double maxVal, uint32_t nBins ) {
			Histogram H( nBins + 1 );
			
			double width = ( maxVal - minVal ) / nBins;
			for(size_t i=0;i<H.bins.size();++i) {
				H.bins[i] = minVal + width * i;
			}
			return H;
		}
		
		static Histogram setupLogBins( double minVal, double maxVal, uint32_t nBins ) {
			Histogram H( nBins + 1 );
			
			double factor = std::pow( maxVal/minVal, 1.0/nBins );
			H.bins[0] = minVal;
			for(size_t i=1;i<H.bins.size();++i) {
				H.bins[i] = H.bins[i-1] * factor;
			}
			return H;
		}

	};

}

#endif
