#ifndef INCLUDED_MQF_FIBONACCI
#define INCLUDED_MQF_FIBONACCI
#include <cmath>
#include <cstdint>
#include "constants.h"

namespace mqf {

	/*
	 * Fibonacci Sequence
	 *
	 * a_0 = 0
	 * a_1 = 1
	 * a_n = a_{n-1} + a_{n-2}
	 *
	 */
	inline uint64_t fibonacci( int n ) {
		uint64_t a = 1, b = 0;
		for(int i=1;i<=n;++i) {
			auto temp = b;
			b += a;
			a = temp;
		}
		return b;
	}

	/*
	 * Binet's formula for the Fibonacci Sequence
	 *
	 */
	inline double fibonacciBinet( int n ) {
		return ( std::pow( Golden, n ) - std::pow( 1.0 - Golden, n ) ) / Sqrt5;
	}

	/*
	 * Fibonacci Ratios
	 *
	 * r_n = \lim_{m\to\infty} a_m / a_{m+n}
	 *
	 */
	inline double fibonacciRatio( int n ) {
		return std::pow( Golden, -n );
	}

}

#endif
