#ifndef INCLUDED_MQF_DISTRIBUTIONS_LOG_NORMAL
#define INCLUDED_MQF_DISTRIBUTIONS_LOG_NORMAL
#include "normal.h"

namespace mqf {
namespace Distributions {

	struct LogNormal : Density<> {
		double mu, sigma2;

		LogNormal( double mu = 0.0, double sigma2 = 1.0 ) : mu(mu), sigma2(sigma2) {}

		double mean() const {
			return std::exp(mu + 0.5*sigma2);
		}

		double median() const {
			return std::exp(mu);
		}

		double mode() const {
			return std::exp(mu - sigma2);
		}

		double variance() const {
			double y = std::exp(sigma2);
			return (y-1.0)*y*std::exp(2.0*mu);
		}

		double operator()( double x ) const {
			double y = std::log(x) - mu;
			return std::exp(-0.5*y*y/sigma2) / (x*sqrt(2.0*Pi*sigma2));
		}

		double derivative( double x ) const {
			double y = std::log(x) - mu;
			return std::exp(-0.5*y*y/sigma2) / (x*sqrt(2.0*Pi*sigma2));
		}

		double cumulative( double x ) const {
			return 0.5 * ( 1.0 + std::erf( (std::log(x) - mu) / sqrt( 2.0 * sigma2 ) ) );
		}

		std::lognormal_distribution<Value> distribution() const {
			return std::lognormal_distribution<Value>( mu, std::sqrt(sigma2) );
		}

	};

	struct StdLogNormal : Density<> {

		double mean() const {
			return std::exp(0.5);
		}

		double median() const {
			return 1.0;
		}

		double mode() const {
			return std::exp(-1.0);
		}

		double variance() const {
			double y = std::exp(1.0);
			return (y-1.0)*y;
		}

		double operator()( double x ) const {
			double y = std::log(x);
			return std::exp(-0.5*y*y) / (x*sqrt(2.0*Pi));
		}

		double derivative( double x ) const {
			double y = std::log(x);
			return std::exp(-0.5*y*y) / (x*sqrt(2.0*Pi));
		}

		double cumulative( double x ) const {
			return 0.5 * ( 1.0 + std::erf( std::log(x) / sqrt( 2.0 ) ) );
		}

		operator LogNormal() const {
			return LogNormal( 0, 1 );
		}

		std::lognormal_distribution<Value> distribution() const {
			return std::lognormal_distribution<Value>( 0, 1 );
		}

	};

	RV<LogNormal> exp( const RV<Normal>& x ) {
		return MakeRV( LogNormal( x.dist.mu, x.dist.sigma2 ) );
	}

	RV<Normal> log( const RV<LogNormal>& x ) {
		return MakeRV( Normal( x.dist.mu, x.dist.sigma2 ) );
	}

	// Assuming independent
	RV<LogNormal> operator*( const RV<LogNormal>& lhs, const RV<LogNormal>& rhs ) {
		return MakeRV( LogNormal( lhs.dist.mu + rhs.dist.mu, lhs.dist.sigma2 + rhs.dist.sigma2 ) );
	}

	RV<LogNormal> operator*( const RV<LogNormal>& lhs, double rhs ) {
		return MakeRV( LogNormal( lhs.dist.mu + std::log(rhs), lhs.dist.sigma2 ) );
	}

	RV<LogNormal> operator*( double lhs, const RV<LogNormal>& rhs ) {
		return rhs * lhs;
	}

	RV<LogNormal> operator/( double lhs, const RV<LogNormal>& rhs ) {
		return MakeRV( LogNormal( -rhs.dist.mu, rhs.dist.sigma2 ) );
	}

}
}

#endif
