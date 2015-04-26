#ifndef INCLUDED_MQF_DISTRIBUTIONS_NORMAL
#define INCLUDED_MQF_DISTRIBUTIONS_NORMAL
#include <cmath>
#include <random>
#include "../distribution.h"
#include "../random_variable.h"
#include "../constants.h"

namespace mqf {
namespace Distributions {

	struct Normal : Density<> {
		double mu, sigma2;

		explicit Normal( double mean = 0.0, double var = 1.0 ) : mu(mean), sigma2(var) {}

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
			return sigma2;
		}

		double operator()( double x ) const {
			double y = x - mu;
			return std::exp(-0.5*y*y/sigma2) / std::sqrt(2.0*Pi*sigma2);
		}

		double derivative( double x ) const {
			return -(x-mu)/sigma2 * (*this)(x);
		}

		double cumulative( double x ) const {
			return 0.5 * ( 1.0 + std::erf( (x-mu)/std::sqrt(2.0*sigma2) ) );
		}

		double log( double x ) const {
			double y = x - mu;
			return -0.5*( y*y/sigma2 + std::log( 2.0*Pi*sigma2 ) );
		}

		std::normal_distribution<Value> distribution() const {
			return std::normal_distribution<Value>( mu, std::sqrt(sigma2) );
		}

		struct Mean {};
		struct Variance {};

	};

	struct StdNormal : Density<> {

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
			return 1.0;
		}

		double operator()( double x ) const {
			double y = x;
			return std::exp(-0.5*y*y) / std::sqrt(2.0*Pi);
		}

		double derivative( double x ) const {
			return -x * (*this)(x);
		}

		double cumulative( double x ) const {
			return 0.5 * ( 1.0 + erf( x/std::sqrt(2.0) ) );
		}

		double log( double x ) const {
			return -0.5*( x*x + std::log( 2.0*Pi ) );
		}

		operator Normal() const {
			return Normal( 0, 1 );
		}
	};

	// Assuming independent
	RV<Normal> operator+( const RV<Normal>& lhs, const RV<Normal>& rhs ) {
		return MakeRV( Normal( lhs.dist.mu + rhs.dist.mu, lhs.dist.sigma2 + rhs.dist.sigma2 ) );
	}

	RV<Normal> operator*( const RV<Normal>& lhs, double rhs ) {
		return MakeRV( Normal( lhs.dist.mu * rhs, lhs.dist.sigma2 * rhs*rhs ) );
	}

	RV<Normal> operator*( double lhs, const RV<Normal>& rhs ) {
		return rhs * lhs;
	}

	RV<Normal> operator+( const RV<Normal>& lhs, double rhs ) {
		return MakeRV( Normal( lhs.dist.mu + rhs, lhs.dist.sigma2 ) );
	}

	RV<Normal> operator+( double lhs, const RV<Normal>& rhs ) {
		return rhs + lhs;
	}

}
}

#endif
