#ifndef INCLUDED_MQF_DISTRIBUTIONS_CAUCHY
#define INCLUDED_MQF_DISTRIBUTIONS_CAUCHY
#include <cmath>
#include <random>
#include "../distribution.h"
#include "../random_variable.h"
#include "../constants.h"

namespace mqf {
namespace Distributions {

	struct Cauchy : Density<> {
		double mu, sigma;
				
		explicit Cauchy( double mu = 0.0, double sigma = 1.0 ) : mu(mu), sigma(sigma) {}

		double median() const {
			return mu;
		}

		double mode() const {
			return mu;
		}

		double operator()( double x ) const {
			double y = ( x - mu ) / sigma;
			return 1.0 / ( Pi * sigma * ( 1.0 + y * y ) );
		}

		double derivative( double x ) const {
			double y = ( x - mu ) / sigma;
			double z = 1.0 + y * y;
			return (-2.0 * y) / ( Pi * sigma*sigma * z*z );
		}

		double cumulative( double x ) const {
			double y = ( x - mu ) / sigma;
			return 0.5 + std::atan( y ) / Pi ;
		}

		std::cauchy_distribution<Value> distribution() const {
			return std::cauchy_distribution<Value>( mu, sigma );
		}

	};

	struct StdCauchy : Density<> {

		double median() const {
			return 0.0;
		}

		double mode() const {
			return 0.0;
		}

		double operator()( double x ) const {
			return 1.0 / ( Pi * ( 1.0 + x * x ) );
		}

		double derivative( double x ) const {
			double z = 1.0 + x * x;
			return (-2.0 * x) / ( Pi * z*z );
		}

		double cumulative( double x ) const {
			return 0.5 + std::atan( x ) / Pi ;
		}

		operator Cauchy() const {
			return Cauchy( 0, 1 );
		}

		std::cauchy_distribution<Value> distribution() const {
			return std::cauchy_distribution<Value>( 0, 1 );
		}

	};

	// Assuming independent
	RV<Cauchy> operator+( const RV<Cauchy>& lhs, const RV<Cauchy>& rhs ) {
		return MakeRV( Cauchy( lhs.dist.mu + rhs.dist.mu, lhs.dist.sigma + rhs.dist.sigma ) );
	}

	RV<Cauchy> operator*( const RV<Cauchy>& lhs, double rhs ) {
		return MakeRV( Cauchy( lhs.dist.mu * rhs, lhs.dist.sigma * std::fabs(rhs) ) );
	}

	RV<Cauchy> operator*( double lhs, const RV<Cauchy>& rhs ) {
		return rhs * lhs;
	}

	RV<Cauchy> operator+( const RV<Cauchy>& lhs, double rhs ) {
		return MakeRV( Cauchy( lhs.dist.mu + rhs, lhs.dist.sigma ) );
	}

	RV<Cauchy> operator+( double lhs, const RV<Cauchy>& rhs ) {
		return rhs + lhs;
	}

	RV<Cauchy> operator/( double lhs, RV<StdCauchy> ) {
		return MakeRV( Cauchy( 0, std::fabs(lhs) ) );
	}

}

/* mle

sum_i (x_i - mu)/( (x_i - mu)^2 + sigma^2 ) = 0

sum_i 1/( (x_i - mu)^2 + sigma^2 ) = N / (2 sigma^2)

*/

}

#endif
