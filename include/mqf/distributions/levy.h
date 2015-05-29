#ifndef INCLUDED_MQF_DISTRIBUTIONS_LEVY
#define INCLUDED_MQF_DISTRIBUTIONS_LEVY
#include <cassert>
#include <cmath>
#include <limits>
#include "../distribution.h"
#include "../random_variable.h"
#include "../constants.h"

namespace mqf {
namespace Distributions {

	struct Levy : Density<> {
		double mu, c;
				
		explicit Levy( double mu = 0.0, double c = 1.0 ) : mu(mu), c(c) {}

		double mean() const {
			return std::numeric_limits<double>::infinity();
		}

		double median() const {
			return mu + c * ( 1.0 / ( 2.0 * InvErfHalf * InvErfHalf ) );
		}

		double mode() const {
			return mu + c / 3.0;
		}

		double variance() const {
			return std::numeric_limits<double>::infinity();
		}

		double operator()( double x ) const {
			double y = (x - mu) / c;
			return std::exp(-0.5 / y) / ( c * y * std::sqrt(2.0*Pi*y) );
		}

		double derivative( double x ) const {
			double y = (x - mu) / c;
			return ( std::exp(-0.5 / y) * ( 1.0 - 3.0 * y ) ) / ( 2.0 * c*c * y*y*y * std::sqrt(2.0*Pi*y) );
		}

		double cumulative( double x ) const {
			return std::erfc( std::sqrt( 0.5 * c / (x-mu) ) );
		}

	};

	struct StdLevy : Density<> {

		double mean() const {
			return std::numeric_limits<double>::infinity();
		}

		double median() const {
			return 1.0 / ( 2.0 * InvErfHalf * InvErfHalf );
		}

		double mode() const {
			return 1.0 / 3.0;
		}

		double variance() const {
			return std::numeric_limits<double>::infinity();
		}

		double operator()( double x ) const {
			return 1.0 / ( x * std::sqrt(2.0*Pi*x) ) * std::exp(-0.5 / x);
		}

		double derivative( double x ) const {
		}

		double cumlative( double x ) const {
			return std::erfc( std::sqrt( 0.5 / x ) );
		}

		operator Levy() const {
			return Levy( 0, 1 );
		}

	};

	RV<Levy> operator*( const RV<Levy>& lhs, double rhs ) {
		return MakeRV( Levy( lhs.dist.mu * rhs, lhs.dist.c * rhs ) );
	}

	RV<Levy> operator*( double lhs, const RV<Levy>& rhs ) {
		return rhs * lhs;
	}

	RV<Levy> operator+( const RV<Levy>& lhs, double rhs ) {
		return MakeRV( Levy( lhs.dist.mu + rhs, lhs.dist.c ) );
	}

	RV<Levy> operator+( double lhs, const RV<Levy>& rhs ) {
		return rhs + lhs;
	}

}

/* mle

1/N sum 1/(x_i - mu)^2 = 3 ( 1/N sum 1/(x_i - mu) )^2

c = harmonicMean(x_i - mu)

*/

}

#endif
