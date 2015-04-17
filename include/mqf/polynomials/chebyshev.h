#ifndef INCLUDED_MQF_POLY_CHEBYSHEV
#define INCLUDED_MQF_POLY_CHEBYSHEV
#include <cmath>
#include "../constants.h"

namespace mqf {
	
	template<int n>
	struct Chebyshev1st {
		double operator()( double x ) const {
			return 2.0 * x * Chebyshev1st<n-1>()(x) - Chebyshev1st<n-2>()(x);
		}
		double derivative( double x ) const {
			return 2.0*( Chebyshev1st<n-1>()(x) + x * Chebyshev1st<n-1>().derivative(x) ) - Chebyshev1st<n-2>().derivative(x);
		}
		double root( int k ) const {
			return std::cos( Pi * double(2*k-1) / (2*n) );
		}
	};

	template<>
	struct Chebyshev1st<0> {
		double operator()( double x ) const {
			return 1.0;
		}
		double derivative( double x ) const {
			return 0.0;
		}
	};

	template<>
	struct Chebyshev1st<1> {
		double operator()( double x ) const {
			return x;
		}
		double derivative( double x ) const {
			return 1.0;
		}
		double root( int ) const {
			return 0.0;
		}
	};

	template<int n>
	struct Chebyshev2nd {
		double operator()( double x ) const {
			return 2.0 * x * Chebyshev2nd<n-1>()(x) - Chebyshev2nd<n-2>()(x);
		}
		double derivative( double x ) const {
			return 2.0*( Chebyshev2nd<n-1>()(x) + x * Chebyshev2nd<n-1>().derivative(x) ) - Chebyshev2nd<n-2>().derivative(x);
		}
		double root( int k ) const {
			return std::cos( Pi * double(k) / (n+1) );
		}
	};

	template<>
	struct Chebyshev2nd<0> {
		double operator()( double x ) const {
			return 1.0;
		}
		double derivative( double x ) const {
			return 0.0;
		}
	};

	template<>
	struct Chebyshev2nd<1> {
		double operator()( double x ) const {
			return 2.0*x;
		}
		double derivative( double x ) const {
			return 2.0;
		}
		double root( int ) const {
			return 0.0;
		}
	};

}

#endif
