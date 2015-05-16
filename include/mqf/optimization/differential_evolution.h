#ifndef INCLUDED_MQF_OPTIMIZATION_DIFFERENTIAL_EVOLUTION
#define INCLUDED_MQF_OPTIMIZATION_DIFFERENTIAL_EVOLUTION
#include <Eigen/Core>
#include <vector>
#include <random>
#include "../pde/aabb.h"

namespace mqf {

	template<typename T,int N>
	struct DifferentialEvolution {
		using Vec = Eigen::Matrix<T,N,1>;

		AABB<T,N> bounds;
		std::vector<Vec> agents;
		std::vector<double> costs;
		std::mt19937 mt;
		std::uniform_int_distribution<int> dist;

		// parameters
		int num_agents = 20,
		    max_evals = 1000;
		double differential_weight = 0.75,
		       crossover_probability = 0.75;

		DifferentialEvolution() : dist(0,N-1) {}

		void initAgents() {
			auto diag = bounds.vector();
			agents.resize( num_agents );
			costs.resize( num_agents );
			for( auto&& x : agents ) {
				x = bounds.minBounds + diag.cwiseProduct( 0.5 * (Vec::Ones() + Vec::Random()) );
			}
		}

		void chooseRandomAgents( int& a, int& b, int& c, int j ) {
			using param_type = std::uniform_int_distribution<int>::param_type;
			std::uniform_int_distribution<int> dist;
			
			dist.param( param_type( 0, num_agents - 2 ) );
			int at = dist(mt);
			if( at == j )
				++at;

			dist.param( param_type( 0, num_agents - 3 ) );
			int bt = dist(mt);
			if( bt == j )
				++bt;
			if( bt == at )
				++bt;
			if( bt == j )
				++bt;
			
			dist.param( param_type( 0, num_agents - 4 ) );
			int ct = dist(mt);
			if( ct == j )
				++ct;
			if( ct == at )
				++ct;
			if( ct == bt )
				++ct;
			if( ct == j )
				++ct;
			if( ct == at )
				++ct;
			if( ct == bt )
				++ct;
			if( ct == j )
				++ct;

			a = at; b = bt; c = ct;
		}

		template<typename F>
		Vec optimize( F&& cost ) {
			initAgents();

			for(int i=0;i<num_agents;++i) {
				costs[i] = cost( agents[i] );
			}

			auto max_its = max_evals / num_agents;
			std::uniform_real_distribution<double> real_dist;
			for(int k=0;k<max_its;++k) {
				for(int j=0;j<num_agents;++j) {
					auto& x = agents[j];
					int a, b, c;
					chooseRandomAgents(a,b,c,j);
					int R = dist(mt);

					Vec y = agents[a] + differential_weight * ( agents[b] - agents[c] );

					for(int i=0;i<N;++i) {
						if( i == R ) {
							continue;
						}
						auto ri = real_dist(mt);
						if( !(ri < crossover_probability) ) {
							y[i] = x[i];
						}
					}
					auto costx = costs[j];
					auto costy = cost(y);
					if( costy < costx ) {
						x = y;
						costs[j] = costy;
					}
				}
				//printf("%d\n",k);
			}
			auto best_agent = std::min_element( costs.begin(), costs.end() );
			auto best_index = std::distance( costs.begin(), best_agent );
			return agents[best_index];
		}


	};
	
}

#endif
