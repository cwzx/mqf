#ifndef INCLUDED_MQF_EULER_MARUYAMA
#define INCLUDED_MQF_EULER_MARUYAMA
#include <random>

namespace mqf {

	template<typename ItoSDE>
	struct EulerMaruyama {
		using Time = double;
		using Value = typename ItoSDE::Value;

		ItoSDE sde;
		std::mt19937 mt;

		EulerMaruyama() = default;

		explicit EulerMaruyama( const ItoSDE& sde ) : sde(sde) {}

		Value advance( const Value& X, Time dt ) {
			std::normal_distribution<> normal( 0.0, dt );
			return X + sde.drift(X) * dt + sde.diffusion(X) * normal(mt);
		}

	};

}

#endif
