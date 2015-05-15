#ifndef INCLUDED_MQF_OPTIMIZATION_DIFFERENTIAL_EVOLUTION
#define INCLUDED_MQF_OPTIMIZATION_DIFFERENTIAL_EVOLUTION
#include <Eigen/Core>
#include <array>
#include <random>
#include "../pde/aabb.h"

namespace mqf {

	template<typename T,int N,int M>
	struct DifferentialEvolution {
		using Vec = Eigen::Matrix<T,N,1>;

		AABB<T,N> bounds;
		std::array<Vec,M> agents;


		void initAgents() {
			auto diag = bounds.vector();
			for( auto&& x : agents ) {
				x = bounds.minBounds + diag * Vec::Random();
			}
		}

		template<typename F>
		Vec optimize( F&& f ) {
			initAgents();
			
			std::mt19973 mt;
			std::uniform_int_distribution<int> dist(0,N-1);
			for( auto&& x : agents ) {
				int a = dist(mt);
				int b = dist(mt);
				int c = dist(mt);
				int R = dist(mt);

				// todo
			}


		}


	}
	
}

#endif
