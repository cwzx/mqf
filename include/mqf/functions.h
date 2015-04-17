#ifndef INCLUDED_MQF_FUNCTIONS
#define INCLUDED_MQF_FUNCTIONS
#include <cstdint>
#include <cmath>
#include <utility>

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

	template<typename T,typename... Ts>
	auto product( T&& x, Ts&&... xs ) {
		return std::forward<T>(x) * product( std::forward<Ts>(xs)... );
	}

	template<typename T>
	auto product( T&& x ) {
		return x;
	}

	template<typename T,typename... Ts>
	auto sum( T&& x, Ts&&... xs ) {
		return std::forward<T>(x) + sum( std::forward<Ts>(xs)... );
	}

	template<typename T>
	auto sum( T&& x ) {
		return x;
	}

	template<typename T>
	auto min( T&& x ) {
		return x;
	}

	template<typename T1,typename T2>
	auto min( T1&& x, T2&& y ) {
		return (x<y)?x:y;
	}

	inline double min( double x, double y ) {
		return std::fmin(x,y);
	}

	template<typename T,typename... Ts>
	auto min( T&& x, Ts&&... xs ) {
		return min( std::forward<T>(x), min( std::forward<Ts>(xs)... ) );
	}

	template<typename T>
	auto max( T&& x ) {
		return x;
	}

	template<typename T1,typename T2>
	auto max( T1&& x, T2&& y ) {
		return (x>y)?x:y;
	}

	inline double max( double x, double y ) {
		return std::fmax(x,y);
	}

	template<typename T,typename... Ts>
	auto max( T&& x, Ts&&... xs ) {
		return max( std::forward<T>(x), max( std::forward<Ts>(xs)... ) );
	}

}

#endif
