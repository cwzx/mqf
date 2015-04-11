#ifndef INCLUDED_MQF_CONJUGATE_PRIORS
#define INCLUDED_MQF_CONJUGATE_PRIORS
#include "../utility.h"
#include "../distributions/gamma.h"
#include "../distributions/exponential.h"
#include "../distributions/beta.h"
#include "../distributions/bernoulli.h"
#include "../distributions/poisson.h"

namespace mqf {

	using namespace Distributions;

	template<typename Data>
	Beta ComputePosterior( const Beta& prior, const Bernoulli& likelihood, const Data& data ) {
		auto sum = accumulate( data, 0.0 );
		auto n = data.size();
		return Beta( prior.alpha + sum, prior.beta - sum + n );
	}

	template<typename Data>
	Gamma ComputePosterior( const Gamma& prior, const Poisson& likelihood, const Data& data ) {
		auto sum = accumulate( data, 0.0 );
		auto n = data.size();
		return Gamma( prior.k + sum, prior.theta / ( n * prior.theta + 1.0 ) );
	}

	template<typename Data>
	Gamma ComputePosterior( const Gamma& prior, const Exponential& likelihood, const Data& data ) {
		auto sum = accumulate( data, 0.0 );
		auto n = data.size();
		return Gamma( prior.k + n, prior.theta / ( sum * prior.theta + 1.0 ) );
	}

}

#endif
