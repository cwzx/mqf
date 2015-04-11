#ifndef INCLUDED_MQF_MILSTEIN
#define INCLUDED_MQF_MILSTEIN
#include <random>

namespace mqf {

	template<typename ItoSDE>
	struct Milstein {
		using Time = double;
		using Value = typename ItoSDE::Value;

		ItoSDE sde;
		std::mt19937 mt;

		Milstein() = default;

		explicit Milstein( const ItoSDE& sde ) : sde(sde) {}

		Value advance( const Value& X, Time dt ) {
			std::normal_distribution<> normal( 0.0, dt );
			double dW = normal(mt);
			Value diffusion = sde.diffusion(X);
			return X + sde.drift(X) * dt
			         + diffusion * dW
			         + diffusion * sde.diffusionD(X) * ( 0.5 * ( dW*dW - dt ) );
		}

	};

}

#endif
