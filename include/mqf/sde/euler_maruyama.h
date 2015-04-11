#ifndef INCLUDED_MQF_EULER_MARUYAMA
#define INCLUDED_MQF_EULER_MARUYAMA
#include <random>

namespace mqf {

	template<typename ItoSDE>
	struct EulerMaruyama {
		using Time = double;
		using State = typename ItoSDE::State;

		ItoSDE sde;
		std::mt19937 mt;

		EulerMaruyama() = default;

		explicit EulerMaruyama( const ItoSDE& sde ) : sde(sde) {}

		State advance( const State& X, Time dt ) {
			std::normal_distribution<> normal( 0.0, dt );
			return X + sde.drift(X) * dt + sde.diffusion(X) * normal(mt);
		}

	};

}

#endif
