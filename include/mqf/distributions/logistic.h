#ifndef INCLUDED_MQF_DISTRIBUTIONS_LOGISTIC
#define INCLUDED_MQF_DISTRIBUTIONS_LOGISTIC
#include <cmath>
#include "../distribution.h"
#include "../random_variable.h"
#include "../constants.h"
#include "../functions.h"

namespace mqf {
namespace Distributions {

	struct Logistic : Density<> {
		double mu, s;
				
		explicit Logistic( double mu = 0.0, double s = 1.0 ) : mu(mu), s(s) {}

		double mean() const {
			return mu;
		}

		double median() const {
			return mu;
		}

		double mode() const {
			return mu;
		}

		double variance() const {
			double a = s * Pi;
			return a*a / 3.0;
		}

		double skewness() const {
			return 0.0;
		}

		double excessKurtosis() const {
			return 1.2;
		}

		double operator()( double x ) const {
			double a = std::exp( -( x - mu )/s );
			double b = 1.0 + a;
			return a / ( s * b*b );
		}

		double cumulative( double x ) const {
			return 1.0 / ( 1.0 + std::exp( -( x - mu )/s ) );
		}

	};

	struct StdLogistic : Density<> {

		double mean() const {
			return 0.0;
		}

		double median() const {
			return 0.0;
		}

		double mode() const {
			return 0.0;
		}

		double variance() const {
			return Pi*Pi / 3.0;
		}

		double skewness() const {
			return 0.0;
		}

		double excessKurtosis() const {
			return 1.2;
		}

		double operator()( double x ) const {
			double a = std::exp( -x );
			return a / square( 1.0 + a );
		}

		double cumulative( double x ) const {
			return 1.0 / ( 1.0 + std::exp( -x ) );
		}

		operator Logistic() const {
			return Logistic(0,1);
		}

	};

	RV<Logistic> operator*( const RV<Logistic>& lhs, double rhs ) {
		return MakeRV( Logistic( lhs.dist.mu * rhs, lhs.dist.s * rhs ) );
	}

	RV<Logistic> operator*( double lhs, const RV<Logistic>& rhs ) {
		return rhs * lhs;
	}

	RV<Logistic> operator+( const RV<Logistic>& lhs, double rhs ) {
		return MakeRV( Logistic( lhs.dist.mu + rhs, lhs.dist.s ) );
	}

	RV<Logistic> operator+( double lhs, const RV<Logistic>& rhs ) {
		return rhs + lhs;
	}

}

/* mle

sum_i tanh( (x_i-mu)/(2 sigma) ) = 0

sum_i x_i tanh( (x_i-mu)/(2 sigma) ) = -N sigma

*/

}

#endif
