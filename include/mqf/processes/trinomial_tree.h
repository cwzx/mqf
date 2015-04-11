#ifndef INCLUDED_MQF_PROCESSES_TRINOMIAL_TREE
#define INCLUDED_MQF_PROCESSES_TRINOMIAL_TREE
#include "../process.h"
#include <cstdint>
#include <random>
#include <cassert>
#include "../distributions/multinomial.h"

namespace mqf {
namespace Processes {
		
	template<typename Value = double>
	struct TrinomialTree : Process<Value,uint32_t> {
		using Trinomial = Distributions::Trinomial;
		double up, down, mid;
		std::mt19937 mt;
		std::discrete_distribution<> dist;

		explicit TrinomialTree( double up = 2.0,
			                    double down = 0.5,
			                    double mid = 1.0,
			                    double p_u = 0.4,
			                    double p_d = 0.4 ) :
			up( up ),
			down( down ),
			mid( mid ),
			dist( { p_u, p_d, 1.0-(p_u+p_d) } )
		{}

		Value advance( Value x, Time dt = 1 ) {
			for(Time t=0;t<dt;++t) {
				auto r = dist(mt);
				if     ( r == 0 ) x *= up;
				else if( r == 1 ) x *= down;
				else if( r == 2 ) x *= mid;
			}
			return x;
		}

		std::vector<double> computeStates( Value initial, Time t ) {
			size_t nStates = ((t+2)*(t+1))/2;
			std::vector<double> states;
			states.reserve( nStates );
				
			uint32_t n_u = 0;
			uint32_t n_m = 0;
			uint32_t n_d = t;
			for(uint32_t i=0;i<nStates;++i) {
				states.push_back( initial * std::pow(up,n_u) * std::pow(mid,n_m) * std::pow(down,n_d) );
				if( n_d > 0 ) {
					--n_d;
					++n_m;
				} else {
					++n_u;
					n_m = 0;
					n_d = t - n_u;
				}
			}
			return states;
		}

		std::vector<double> computeProbabilities( Time t ) {
			size_t nProbs = ((t+2)*(t+1))/2;
			std::vector<double> probs;
			probs.reserve( nProbs );

			Trinomial T( t, dist.probabilities() );

			uint32_t n_u = 0;
			uint32_t n_m = 0;
			uint32_t n_d = t;
			for(uint32_t i=0;i<nProbs;++i) {
				probs.push_back( T( { n_u, n_d, n_m } ) );
				if( n_d > 0 ) {
					--n_d;
					++n_m;
				} else {
					++n_u;
					n_m = 0;
					n_d = t - n_u;
				}
			}
			return probs;
		}

		std::vector<double> computeStateTree( Value initial, Time t ) {
			size_t nTotalStates = ((t+3)*(t+2)*(t+1))/6;
			std::vector<double> states;
			states.reserve( nTotalStates );
				
			for(uint32_t i=0;i<=t;++i) {
				uint32_t nStates = ((i+2)*(i+1))/2;
				uint32_t n_u = 0;
				uint32_t n_m = 0;
				uint32_t n_d = i;
				for(uint32_t j=0;j<nStates;++j) {
					states.push_back( initial * std::pow(up,n_u) * std::pow(mid,n_m) * std::pow(down,n_d) );
					if( n_d > 0 ) {
						--n_d;
						++n_m;
					} else {
						++n_u;
						n_m = 0;
						n_d = i - n_u;
					}
				}
			}
			return states;
		}

		std::vector<double> computeProbabilityTree( Time t ) {
			size_t nTotalProbs = ((t+3)*(t+2)*(t+1))/6;
			std::vector<double> probs;
			probs.reserve( nTotalProbs );

			Trinomial T( 0, dist.probabilities() );

			for(uint32_t i=0;i<=t;++i) {
				T.n = i;
				uint32_t nProbs = ((i+2)*(i+1))/2;
				uint32_t n_u = 0;
				uint32_t n_m = 0;
				uint32_t n_d = i;
				for(uint32_t j=0;j<nProbs;++j) {
					probs.push_back( T( { n_u, n_d, n_m } ) );
					if( n_d > 0 ) {
						--n_d;
						++n_m;
					} else {
						++n_u;
						n_m = 0;
						n_d = i - n_u;
					}
				}
			}
			return probs;
		}

		void writeStateTree( Value initial, Time t, const char* filename ) {
			ofstream out(filename);
				
			for(uint32_t i=0;i<=t;++i) {
				uint32_t nStates = ((i+2)*(i+1))/2;
				uint32_t n_u = 0;
				uint32_t n_m = 0;
				uint32_t n_d = i;
				for(uint32_t j=0;j<nStates;++j) {
					out <<  initial * std::pow(up,n_u) * std::pow(mid,n_m) * std::pow(down,n_d) << ",";
					if( n_d > 0 ) {
						--n_d;
						++n_m;
					} else {
						++n_u;
						n_m = 0;
						n_d = i - n_u;
					}
				}
				out << endl;
			}
		}

		void writeProbabilityTree( Time t, const char* filename ) {
			ofstream out(filename);

			Trinomial T( 0, dist.probabilities() );

			for(uint32_t i=0;i<=t;++i) {
				T.n = i;
				uint32_t nProbs = ((i+2)*(i+1))/2;
				uint32_t n_u = 0;
				uint32_t n_m = 0;
				uint32_t n_d = i;
				for(uint32_t j=0;j<nProbs;++j) {
					out <<  T( { n_u, n_d, n_m } ) << ",";
					if( n_d > 0 ) {
						--n_d;
						++n_m;
					} else {
						++n_u;
						n_m = 0;
						n_d = i - n_u;
					}
				}
				out << endl;
			}
		}

		static TrinomialTree<Value> MakeBoyle( double mu, double sigma, double delta ) {
			assert( delta < 2.0 * sigma*sigma / (mu*mu) );
			double a = std::exp( 0.5 * mu * delta );
			double b = std::exp( sigma * std::sqrt( 0.5 * delta ) );
			double up = b * b;
			double rb = 1.0 / b;
			double c = (a - rb) / ( b - rb );
			double d = (b - a) / ( b - rb );
			double p_u = c*c;
			double p_d = d*d;
			return TrinomialTree<Value>( up, 1.0/up, 1.0, p_u, p_d );
		}

	};
	
}
}

#endif
