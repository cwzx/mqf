#ifndef INCLUDED_MQF_POLY_HERMITE
#define INCLUDED_MQF_POLY_HERMITE

namespace mqf {
	
	template<int n>
	struct Hermite {
		double operator()( double x ) const {
			return x * Hermite<n-1>()(x) - (n-1) * Hermite<n-2>()(x);
		}
		double derivative( double x ) const {
			return n * Hermite<n-1>()(x);
		}
	};

	template<>
	struct Hermite<0> {
		double operator()( double x ) const {
			return 1.0;
		}
		double derivative( double x ) const {
			return 0.0;
		}
	};

	template<>
	struct Hermite<1> {
		double operator()( double x ) const {
			return x;
		}
		double derivative( double x ) const {
			return 1.0;
		}
	};

	

}

#endif
