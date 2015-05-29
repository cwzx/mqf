#ifndef INCLUDED_MQF_DISTRIBUTIONS_WALD
#define INCLUDED_MQF_DISTRIBUTIONS_WALD
#include "normal.h"

namespace mqf {
namespace Distributions {

	struct Wald : Density<> {
		double mu, lambda;
				
		explicit Wald( double mu = 1.0, double lambda = 1.0 ) : mu(mu), lambda(lambda) {}

		double mean() const {
			return mu;
		}

		double mode() const {
			double r = 1.5 * mu / lambda;
			return mu * ( std::sqrt( 1.0 + r*r ) - r );
		}

		double variance() const {
			return mu*mu*mu/lambda;
		}

		double skewness() const {
			return 3.0 * std::sqrt( mu / lambda );
		}

		double excessKurtosis() const {
			return 15.0 * mu / lambda;
		}

		double operator()( double x ) const {
			double y = ( x - mu );
			return std::sqrt( lambda / ( 2.0 * Pi * x*x*x ) ) * std::exp( -0.5 * lambda * y*y / ( mu*mu * x ) );
		}

		double cumulative( double x ) const {
			double y = ( x - mu );
			double a = std::sqrt( lambda / x );
			double b = x / mu; 
			StdNormal N;
			return N.cumulative( a * (b - 1) ) + std::exp( 2.0 * lambda/mu ) * N.cumulative( -a * (b + 1) );
		}

	};

	RV<Wald> operator*( const RV<Wald>& lhs, double rhs ) {
		return MakeRV( Wald( lhs.dist.mu * rhs, lhs.dist.lambda * rhs ) );
	}

	RV<Wald> operator*( double lhs, const RV<Wald>& rhs ) {
		return rhs * lhs;
	}

}

	template<>
	struct MLE<Distributions::Wald> {
		using Dist = Distributions::Wald;
		template<typename It>
		Dist operator()( It p1, It p2 ) const {
			auto mu = sampleMean(p1,p2);
			auto lambda = 1.0 / ( meanReciprocal(p1,p2) - 1.0 / mu );
			return Dist( mu, lambda );
		}
	};
	
	template<typename>
	struct MomentEstimation;

	template<>
	struct MomentEstimation<Distributions::Wald> {
		using Dist = Distributions::Wald;
		template<typename It>
		Dist operator()( It p1, It p2 ) const {
			auto mu = sampleMean(p1,p2);
			auto var = sampleVariance(p1,p2,mu);
			return Dist( mu, mu*mu*mu/var );
		}
	};

}

#endif
