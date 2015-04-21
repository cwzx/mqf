#ifndef INCLUDED_MQF_FOKKERPLANCK
#define INCLUDED_MQF_FOKKERPLANCK
#include <cstdint>

namespace mqf {

	template<typename Drift,typename Diffusion>
	struct FokkerPlanck {
		Drift mu;
		Diffusion sigma;

		template<typename F>
		struct RHS;

		template<typename F>
		RHS<F> operator()( F&& f ) const {
			return RHS<F>( std::forward<F>(f), mu, sigma );
		}

	};
	template<typename Drift,typename Diffusion,typename F>
	struct FokkerPlanck<Drift,Diffusion>::RHS {
		Drift mu;
		Diffusion sigma;
		const F& f;

		explicit RHS( const F& f, Drift drift, Diffusion diffusion ) : f(f), mu(mu), sigma(sigma) {}

		double operator()( double x, double t ) const {
			return -div( f * mu )(x,t) + 0.5 * laplacian( f * sigma*sigma )(x,t);
		}

	};

}

#endif
