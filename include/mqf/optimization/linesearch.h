#ifndef INCLUDED_MQF_OPTIMIZATION_LINESEARCH
#define INCLUDED_MQF_OPTIMIZATION_LINESEARCH
#include <cstdint>
#include <cmath>
#include "../utility.h"

namespace mqf {

	/*
	 * Line Search
	 *
	 * A line search seeks a local minimum of a one-dimensional
	 * continuously differentiable cost function S(alpha) such that alpha >= 0.
	 *
	 * This is primarily used by each step of n-dimensional nonlinear optimization methods.
	 *
	 * This implementation establishes a bounding interval [a,b] with S'(a) < 0 and S'(b) > 0
	 * and then iteratively refines the interval using the secant method.
	 *
	 * The Wolfe conditions are tested for early termination.
	 *
	 */
	struct LineSearch {
		double alpha = 1.0e-2,    // the previous solution
		       alphaMax = 1.0e6,  // upper limit on solution
		       mu = 1.0e-4,       // parameter for first Wolfe condition
		       eta = 0.9;         // parameter for second Wolfe condition

		void reset() {
			alpha = 1.0e-2;
		}

		/*
		 * Perform the search.
		 *
		 * The cost function and its derivative must be provided as callable objects:
		 * e.g. function object, lambda, function pointer.
		 *
		 * The return value is an approximate local minimum.
		 *
		 */
		template<typename S,typename DS>
		double search( S&& cost, DS&& gradient ) {
			static const double ratio = 2.0;
			static const int maxIterations = 5;

			double g0 = gradient(0.0);
			if( g0 >= 0.0 ) {
				return 0.0;
			}
			double S0 = cost(0.0);
			
			// the lower bound is set to zero
			double a = 0.0;
			double ga = g0;

			// the upper bound is initially set to the last valid solution...
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

			// refine the interval using the secant method
			double c, Sc, gc;
			for(int i=0;i<maxIterations;++i) {
				c = secant(a,b,ga,gb);
				gc = gradient(c);
				Sc = cost(c);

				// if the Wolfe conditions are satisfied, the current value is good enough
				if( wolfe(S0,g0,c,Sc,gc) ) {
					return alpha = c;
				}

				// update the lower or upper bound depending on the sign of gc
				if( gc < 0.0 ) {
					a = c;
					ga = gc;
				} else if( gc > 0.0 ) {
					b = c;
					gb = gc;
				} else break;
			}

			// sanity check
			if( Sc > S0 ) {
				return 0.0;
			}
			return alpha = c;
		}

	protected:

		/*
		 * The strong Wolfe conditions
		 *
		 */
		bool wolfe( double S0, double g0, double c, double Sc, double gc ) const {

			double c1 = (Sc - S0) / (c * g0);

			if( c1 < 0.0 || mu < c1 ) return false;

			double c2 = std::fabs( gc / g0 );

			if( eta < c2 ) return false;

			return true;
		}

		static double secant( double a, double b, double ga, double gb ) {
			return a - ga * (b - a) / (gb - ga);
		}

	};

}

#endif
