#ifndef INCLUDED_MQF_DISTRIBUTIONS_EXPONENTIAL
#define INCLUDED_MQF_DISTRIBUTIONS_EXPONENTIAL
#include "gamma.h"
#include "../random_variable.h"

namespace mqf {
namespace Distributions {

	struct Exponential : Density<> {
		double lambda;
				
		explicit Exponential( double rate = 1.0 ) : lambda(rate) {}

		double mean() const {
			return 1.0 / lambda;
		}

		double median() const {
			return std::log(2.0) / lambda;
		}

		double mode() const {
			return 0.0;
		}

		double variance() const {
			return 1.0 / ( lambda * lambda );
		}

		double operator()( double x ) const {
			return lambda * std::exp(-lambda*x);
		}

		double derivative( double x ) const {
			return -lambda*lambda * std::exp(-lambda*x);
		}

		double cumulative( double x ) const {
			return 1.0 - std::exp(-lambda*x);
		}

		operator Gamma() const {
			return Gamma( 1, 1.0/lambda );
		}

		std::exponential_distribution<Value> distribution() const {
			return std::exponential_distribution<Value>( lambda );
		}

	};

	// Assuming independent
	RV<Exponential> min( const RV<Exponential>& lhs, const RV<Exponential>& rhs ) {
		return MakeRV( Exponential( lhs.dist.lambda + rhs.dist.lambda ) );
	}

	RV<Exponential> operator*( const RV<Exponential>& lhs, double rhs ) {
		return MakeRV( Exponential( lhs.dist.lambda / rhs ) );
	}

	RV<Exponential> operator*( double lhs, const RV<Exponential>& rhs ) {
		return rhs * lhs;
	}

}
}

#endif
