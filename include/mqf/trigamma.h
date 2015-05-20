#ifndef INCLUDED_MQF_TRIGAMMA
#define INCLUDED_MQF_TRIGAMMA
#include <cmath>
#include <cassert>

namespace mqf {

	/*
	 * 15th-order asymptotic expansion of the trigamma function
	 *
	 */
	inline double trigamma_asymptotic( double x ) {
		//assert( x > 1.2 );
		double rxx = 1.0 / (x*x);
		double yp = 1.0 / x;
		double sum = yp + 0.5 * rxx;

		yp *= rxx;
		sum += ( 1.0/6.0 ) * yp;
		
		yp *= rxx;
		sum -= ( 1.0/30.0 ) * yp;

		yp *= rxx;
		sum += ( 1.0/42.0 ) * yp;

		yp *= rxx;
		sum -= ( 1.0/30.0 ) * yp;

		yp *= rxx;
		sum += ( 5.0/66.0 ) * yp;

		yp *= rxx;
		sum -= ( 691.0/2730.0 ) * yp;

		yp *= rxx;
		sum += ( 7.0/6.0 ) * yp;

		return sum;
	}

	/*
	 * Trigamma function
	 *
	 * Uses the asymptotic expansion for x >= 11.
	 *
	 * For x < 11 uses the recurrence relation.
	 *
	 */
	inline double trigamma( double x ) {
		if( x >= 11.0 ) {
			return trigamma_asymptotic( x );
		}
		auto diff = std::ceil( 11.0 - x );
		x += diff;
		auto psi = trigamma_asymptotic( x );
		int N = int(diff);
		for(int i=0;i<N;++i) {
			--x;
			psi += 1.0 / (x*x);
		}
		return psi;
	}

}

#endif
