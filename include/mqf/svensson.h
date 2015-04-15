#ifndef INCLUDED_MQF_SVENSSON
#define INCLUDED_MQF_SVENSSON
#include <cmath>

namespace mqf {

	struct Svensson {
		double z1, z2, z3, z4, z5, z6;

		explicit Svensson( double z1 = 1.0,
		                   double z2 = 1.0,
		                   double z3 = 1.0,
		                   double z4 = 1.0,
		                   double z5 = 1.0,
		                   double z6 = 1.0 ) :
			z1(z1),
			z2(z2),
			z3(z3),
			z4(z4),
			z5(z5),
			z6(z6)
		{}

		double operator()( double x ) const {
			return z1 + (z2 + z3 * x) * std::exp( -z4 * x ) + z5 * x * std::exp( -z6 * x );
		}

		double derivative( double x ) const {
			return ( z3 - z4 * (z2 + z3 * x) ) * std::exp( -z4 * x ) + z5 * ( 1.0 - z6 * x ) * std::exp( -z6 * x );
		}

		Svensson& operator+=( double rhs ) {
			z1 += rhs;
			return *this;
		}

		Svensson operator+( double rhs ) const {
			auto r = *this;
			r += rhs;
			return r;
		}

		Svensson& operator-=( double rhs ) {
			z1 -= rhs;
			return *this;
		}

		Svensson operator-( double rhs ) const {
			auto r = *this;
			r -= rhs;
			return r;
		}

		Svensson& operator*=( double rhs ) {
			z1 *= rhs;
			z2 *= rhs;
			z3 *= rhs;
			z5 *= rhs;
			return *this;
		}

		Svensson operator*( double rhs ) const {
			auto r = *this;
			r *= rhs;
			return r;
		}

		Svensson& operator/=( double rhs ) {
			z1 /= rhs;
			z2 /= rhs;
			z3 /= rhs;
			z5 /= rhs;
			return *this;
		}

		Svensson operator/( double rhs ) const {
			auto r = *this;
			r /= rhs;
			return r;
		}

	};

}

#endif
