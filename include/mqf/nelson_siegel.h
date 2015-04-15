#ifndef INCLUDED_MQF_NELSON_SIEGEL
#define INCLUDED_MQF_NELSON_SIEGEL
#include "svensson.h"

namespace mqf {

	struct NelsonSiegel {
		double z1, z2, z3, z4;

		explicit NelsonSiegel( double z1 = 1.0,
		                       double z2 = 1.0,
		                       double z3 = 1.0,
		                       double z4 = 1.0 ) :
			z1(z1),
			z2(z2),
			z3(z3),
			z4(z4)
		{}

		double operator()( double x ) const {
			return z1 + (z2 + z3 * x) * std::exp( -z4 * x );
		}

		double derivative( double x ) const {
			return ( z3 - z4 * (z2 + z3 * x) ) * std::exp( -z4 * x );
		}

		NelsonSiegel& operator+=( double rhs ) {
			z1 += rhs;
			return *this;
		}

		NelsonSiegel operator+( double rhs ) const {
			auto r = *this;
			r += rhs;
			return r;
		}

		NelsonSiegel& operator-=( double rhs ) {
			z1 -= rhs;
			return *this;
		}

		NelsonSiegel operator-( double rhs ) const {
			auto r = *this;
			r -= rhs;
			return r;
		}

		NelsonSiegel& operator*=( double rhs ) {
			z1 *= rhs;
			z2 *= rhs;
			z3 *= rhs;
			return *this;
		}

		NelsonSiegel operator*( double rhs ) const {
			auto r = *this;
			r *= rhs;
			return r;
		}

		NelsonSiegel& operator/=( double rhs ) {
			z1 /= rhs;
			z2 /= rhs;
			z3 /= rhs;
			return *this;
		}

		NelsonSiegel operator/( double rhs ) const {
			auto r = *this;
			r /= rhs;
			return r;
		}

		operator Svensson() const {
			return Svensson( z1, z2, z3, z4, 0, 0 );
		}

	};

}

#endif
