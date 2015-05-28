#ifndef INCLUDED_MQF_PDE_BLACK_SCHOLES
#define INCLUDED_MQF_PDE_BLACK_SCHOLES
#include "pde.h"

namespace mqf {

	/*
	 * Black--Scholes equation
	 *
	 * \partial_t V(S,t) = r S - r S \partial_S V(S,t) - 0.5 sigma^2 S^2 \partial_{SS} V(S,t)
	 *
	 */
	struct BlackScholes {
		double r, sigma;

		template<typename F>
		struct RHS;

		explicit BlackScholes( double r = 0.01, double sigma = 0.3 ) : r(r), sigma(sigma) {}

		template<typename F>
		RHS<F> operator()( F&& V ) const {
			return RHS<F>( std::forward<F>(V), r, sigma );
		}

	};

	template<typename F>
	struct BlackScholes::RHS {
		double r, sigma;
		const F& V;

		explicit RHS( const F& V, double r = 0.01, double sigma = 0.3 ) : V(V), r(r), sigma(sigma) {}

		double operator()( double S, double ) const {
			return r * V(S) - r * S * partial<0,1>(V)(S) - 0.5*sigma*sigma * S*S * partial<0,2>(V)(S);
		}

	};

}

#endif
