#ifndef INCLUDED_MQF_OPTIMIZATION_LINESEARCH
#define INCLUDED_MQF_OPTIMIZATION_LINESEARCH
#include <cstdint>
#include <iostream>
#include <cmath>
#include "../utility.h"

namespace mqf {

	struct LineSearch {
		double alpha = 1.0e-2,
		       alphaMax = 1.0e6,
		       mu = 0.00001,
		       eta = 0.9;

		void reset() {
			alpha = 1.0e-2;
		}

		template<typename S,typename DS>
		double search( const S& cost, const DS& costDeriv ) {
			static const double ratio = 2.0;
			static const int maxIterations = 5;

			double g0 = costDeriv(0.0);
			
			if( g0 >= 0.0 ) {
				return 0.0;
			}

			double S0 = cost(0.0);
			double a = 0.0;
			double ga = g0;
			double c = alpha / ratio;
			double gc;
			do {
				c = clamp( c * ratio, 0.0, alphaMax );
				gc = costDeriv(c);
				if( c == alphaMax ) {
					if( gc < 0.0 ) {
						if( cost(c) < S0 ) {
							alpha = c;
							return alpha;
						}
						return 0.0;
					}
					break;
				}
			} while( gc < 0.0 );

			double b = c;
			double gb = gc;
			double Sc = 0.0;
			for(int i=0;i<maxIterations;++i) {
				c = secant(a,b,ga,gb);
				gc = costDeriv(c);
				Sc = cost(c);

				if( wolfe(S0,g0,Sc,gc) ) {
					alpha = c;
					return alpha;
				}
				if( gc < 0.0 ) {
					a = c;
					ga = gc;
				} else if( gc > 0.0 ) {
					b = c;
					gb = gc;
				} else break;
			}
			if( Sc < S0 ) {
				alpha = c;
				return alpha;
			}
			return 0.0;
		}

	protected:

		bool wolfe( double S0, double g0, double Sc, double gc ) const {

			double c1 = Sc - S0;

			if( c1 > 0.0 ) return false;

			if( g0 == 0.0 ) return false;

			c1 /= g0;

			if( c1 > mu ) return false;

			double d2 = std::abs( g0 );

			if( d2 == 0.0 ) return false;

			double c2 = std::abs( gc ) / d2;

			if( c2 > eta ) return false;

			return true;
		}

		static double secant( double a, double b, double ga, double gb ) {
			return a - ga*(b-a)/(gb-ga);
		}

	};

}

#endif
