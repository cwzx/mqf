#ifndef INCLUDED_MQF_OPTIMIZATION_LINESEARCH
#define INCLUDED_MQF_OPTIMIZATION_LINESEARCH
#include <cstdint>
#include <cmath>
#include "../utility.h"

namespace mqf {

	/*
	 * Line Search
	 *
	 * A line search seeks to find a local minimum of a one-dimensional
	 * continuously differentiable cost function f(t) such that t >= 0.
	 *
	 * This is primarily used in each step of n-dimensional nonlinear optimization methods.
	 *
	 * This implementation establishes a bounding interval [a,b] with f'(a) < 0 and f'(b) > 0
	 * and then iteratively refines the interval using the secant method.
	 *
	 * The Wolfe conditions are tested for early termination.
	 *
	 */
	struct LineSearch {
		double alpha = 1.0e-2,
		       alphaMax = 1.0e6,
		       mu = 0.00001,
		       eta = 0.9;

		void reset() {
			alpha = 1.0e-2;
		}

		template<typename S,typename DS>
		double search( const S& cost, const DS& gradient ) {
			static const double ratio = 2.0;
			static const int maxIterations = 5;

			double g0 = gradient(0.0);
			if( g0 >= 0.0 ) {
				return 0.0;
			}
			double S0 = cost(0.0);
			
			// The lower bound is set to zero
			double a = 0.0;
			double ga = g0;

			// The upper bound is initially set to the last valid solution...
			double b = alpha;
			double gb = gradient(b);

			// ...and increased until it becomes valid
			while( gb < 0.0 ) {
				b *= ratio;
				gb = gradient(b);

				// enforce a maximum limit
				if( b >= alphaMax ) {
					if( gb > 0.0 ) {
						if( cost(b) < S0 ) {
							return alpha = b;
						}
						return 0.0;
					}
					break;
				}
			};

			// Refine the interval using the secant method
			double c, Sc, gc;
			for(int i=0;i<maxIterations;++i) {
				c = secant(a,b,ga,gb);
				gc = gradient(c);
				Sc = cost(c);

				// If the Wolfe conditions are satisfied, the current value is good enough
				if( wolfe(S0,g0,Sc,gc) ) {
					return alpha = c;
				}

				// Update the lower or upper bound depending on the sign of gc
				if( gc < 0.0 ) {
					a = c;
					ga = gc;
				} else if( gc > 0.0 ) {
					b = c;
					gb = gc;
				} else break;
			}

			// Sanity check
			if( Sc < S0 ) {
				return alpha = c;
			}
			return 0.0;
		}

	protected:

		bool wolfe( double S0, double g0, double Sc, double gc ) const {

			double c1 = (Sc - S0) / g0;

			if( c1 > mu || c1 < 0.0 ) return false;

			double c2 = std::fabs( gc / g0 );

			if( c2 > eta ) return false;

			return true;
		}

		static double secant( double a, double b, double ga, double gb ) {
			return a - ga * (b - a) / (gb - ga);
		}

	};

}

#endif
