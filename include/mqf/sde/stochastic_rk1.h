#ifndef INCLUDED_MQF_STOCHASTIC_RK1
#define INCLUDED_MQF_STOCHASTIC_RK1
#include <random>

namespace mqf {

	template<typename ItoSDE>
	struct StochasticRK1 {
		using Time = double;
		using Value = typename ItoSDE::Value;

		ItoSDE sde;
		std::mt19937 mt;

		StochasticRK1() = default;

		explicit StochasticRK1( const ItoSDE& sde ) : sde(sde) {}

		Value advance( const Value& X, Time dt ) {
			std::normal_distribution<> normal( 0.0, dt );
			double dW = normal(mt);
			double sqrtdt = sqrt(dt);
			Value drift = sde.drift(X);
			Value diffusion = sde.diffusion(X);
			Value Y = X + drift * dt + diffusion * sqrtdt;
			return X + drift * dt
			         + diffusion * dW
			         + ( sde.diffusion(Y) - diffusion ) * ( 0.5 * ( dW*dW - dt ) / sqrtdt );
		}

	};
}

#endif
