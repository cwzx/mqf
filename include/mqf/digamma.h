#ifndef INCLUDED_MQF_DIGAMMA
#define INCLUDED_MQF_DIGAMMA
#include <cmath>
#include <cassert>

namespace mqf {

	/*
	 * 14th-order asymptotic expansion of the digamma function
	 *
	 */
	inline double digamma_asymptotic( double x ) {
		//assert( x > 1.2 );
		double rxx = 1.0 / (x*x);
		double yp = rxx;
		double sum = std::log( x ) - 0.5 / x;

		sum -= ( 1.0/12.0 ) * yp;
		
		yp *= rxx;
		sum += ( 1.0/120.0 ) * yp;

		yp *= rxx;
		sum -= ( 1.0/252.0 ) * yp;

		yp *= rxx;
		sum += ( 1.0/240.0 ) * yp;

		yp *= rxx;
		sum -= ( 5.0/660.0 ) * yp;

		yp *= rxx;
		sum += ( 691.0/32760.0 ) * yp;

		yp *= rxx;
		sum -= ( 1.0/12.0 ) * yp;

		return sum;
	}

	/*
	 * Digamma function, psi(x)
	 *
	 * Uses the asymptotic expansion for x >= 9.
	 *
	 * For x < 9 uses the recurrence relation to express psi(x) in terms of psi(x + c), where x + c >= 9.
	 *
	 */
	inline double digamma( double x ) {
		if( x >= 9.0 ) {
			return digamma_asymptotic( x );
		}
		auto diff = std::ceil( 9.0 - x );
		x += diff;
		auto psi = digamma_asymptotic( x );
		int N = int(diff);
		for(int i=0;i<N;++i) {
			--x;
			psi -= 1.0 / x;
		}
		return psi;
	}

}

#endif
