#ifndef INCLUDED_MQF_FUNCTIONS
#define INCLUDED_MQF_FUNCTIONS
#include <cstdint>
#include <cmath>

namespace mqf {

	inline double binomial( uint32_t n, uint32_t k ) {
		if( k == 0 || n == k ) return 1.0;
		if( k > n ) return 0.0;
		if( k > (n - k) ) k = n - k;
		if( k == 1 ) return (double)n;

		double b = 1.0;

		for(uint32_t i=1;i<=k;++i) {
			b *= (double)(n - (k - i));
			if( b < 0.0 ) return -1.0;
			b /= (double)i;
		}
		return b;
	}

	inline double logFactorial( uint32_t n ) {
		return std::lgamma( n + 1 );
	}

	inline double delta( uint32_t i, uint32_t j ) {
		return (i==j)?1.0:0.0;
	}

	inline double factorial( uint32_t n ) {
		double k = 1;
		for(uint32_t i=2;i<=n;++i) {
			k *= i;
		}
		return k;
	}

	inline double beta( double x, double y ) {
		return std::exp( std::lgamma(x) + std::lgamma(y) - std::lgamma(x+y) );
	}

	inline double logBeta( double x, double y ) {
		return std::lgamma(x) + std::lgamma(y) - std::lgamma(x+y);
	}

	template<typename T,typename F>
	void plot( const char* filename, T lower, T upper, uint32_t N, F&& f ) {
		std::ofstream out(filename);

		T delta = ( upper - lower ) / ( N - 1 );
		T x = lower;
		for(uint32_t i=0;i<N;++i) {
			out << x << "," << std::forward<F>(f)(x) << endl;
			x += delta;
		}
		
	}

}

#endif
