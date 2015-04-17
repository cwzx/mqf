#ifndef INCLUDED_MQF_POLY_LEGENDRE
#define INCLUDED_MQF_POLY_LEGENDRE

namespace mqf {
	
	template<int n>
	struct Legendre {
		double operator()( double x ) const {
			return double( (2*n-1) * x * Legendre<n-1>()(x) - (n-1) * Legendre<n-2>()(x) ) / n;
		}
		double derivative( double x ) const {
			return double( (2*n-1) * ( Legendre<n-1>()(x) + x * Legendre<n-1>().derivative(x) ) - (n-1) * Legendre<n-2>().derivative(x) ) / n;
		}
	};

	template<>
	struct Legendre<0> {
		double operator()( double x ) const {
			return 1.0;
		}
		double derivative( double x ) const {
			return 0.0;
		}
	};

	template<>
	struct Legendre<1> {
		double operator()( double x ) const {
			return x;
		}
		double derivative( double x ) const {
			return 1.0;
		}
	};

	template<>
	struct Legendre<2> {
		double operator()( double x ) const {
			return 1.5 * x*x - 0.5;
		}
		double derivative( double x ) const {
			return 3.0*x;
		}
	};

	template<>
	struct Legendre<3> {
		double operator()( double x ) const {
			return ( 2.5 * x*x - 1.5 )*x;
		}
		double derivative( double x ) const {
			return 7.5 * x*x - 1.5;
		}
	};

}

#endif
