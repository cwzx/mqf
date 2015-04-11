#ifndef INCLUDED_MQF_PROCESSES_BINOMIAL_TREE
#define INCLUDED_MQF_PROCESSES_BINOMIAL_TREE
#include "../process.h"
#include "../distributions/binomial.h"
#include <cstdint>
#include <random>
#include <vector>
#include <fstream>

namespace mqf {

	namespace Processes {
		
		template<typename Value = double>
		struct BinomialTree : Process<Value,uint32_t> {
			double up, down;
			std::mt19937 mt;
			std::bernoulli_distribution dist;

			explicit BinomialTree( double up = 2.0,
			                       double down = 0.5,
			                       double p = 0.5 ) :
				up( up ),
				down( down ),
				dist( p )
			{}

			Value advance( Value x, Time dt = 1 ) {
				for(Time t=0;t<dt;++t) {
					x *= (dist(mt) ? up : down);
				}
				return x;
			}

			std::vector<double> computeStates( Value initial, Time t ) {
				size_t nStates = t + 1;
				std::vector<double> states;
				states.reserve( nStates );
				
				for(size_t i=0;i<nStates;++i) {
					states.push_back( initial * std::pow(up,i) * std::pow(down,t-i) );
				}
				return states;
			}

			std::vector<double> computeProbabilities( Time t ) {
				size_t nProbs = t + 1;
				std::vector<double> probs;
				probs.reserve( nProbs );

				Binomial B( t, dist.p() );

				for(size_t i=0;i<nProbs;++i) {
					probs.push_back( B( i ) );
				}
				return probs;
			}

			std::vector<double> computeStateTree( Value initial, Time t ) {
				size_t nTotalStates = ((t+1)*(t+2))/2;
				std::vector<double> states;
				states.reserve( nTotalStates );
				
				for(size_t i=0;i<=t;++i) {
					size_t nStates = i + 1;
					for(size_t j=0;j<nStates;++j) {
						states.push_back( initial * std::pow(up,j) * std::pow(down,i-j) );
					}
				}
				return states;
			}

			std::vector<double> computeProbabilityTree( Time t ) {
				size_t nTotalProbs = ((t+1)*(t+2))/2;
				std::vector<double> probs;
				probs.reserve( nTotalProbs );

				double p = dist.p();

				for(size_t i=0;i<=t;++i) {
					Binomial B( i, p );
					size_t nProbs = i + 1;
					for(size_t j=0;j<nProbs;++j) {
						probs.push_back( B( j ) );
					}
				}
				return probs;
			}

			void writeStateTree( Value initial, Time t, const char* filename ) {
				ofstream out(filename);
				
				for(size_t i=0;i<=t;++i) {
					size_t nStates = i + 1;
					for(size_t j=0;j<nStates;++j) {
						out <<  initial * std::pow(up,j) * std::pow(down,i-j) << ",";
					}
					out << endl;
				}
			}

			void writeProbabilityTree( Time t, const char* filename ) {
				ofstream out(filename);

				double p = dist.p();

				for(size_t i=0;i<=t;++i) {
					Binomial B( i, p );
					size_t nProbs = i + 1;
					for(size_t j=0;j<nProbs;++j) {
						out <<  B( j ) << ",";
					}
					out << endl;
				}
			}

			static BinomialTree<Value> MakeCRR( double sigma, double delta ) {
				double up = std::exp( sigma * std::sqrt(delta) );
				return BinomialTree<Value>( up, 1.0/up, 0.5 );
			}

			static BinomialTree<Value> MakeJarrowRudd( double mu, double sigma, double delta ) {
				double a = std::exp( (mu-0.5*sigma*sigma)*delta );
				double b = std::exp( sigma * std::sqrt(delta) );
				return BinomialTree<Value>( a*b, a/b, 0.5 );
			}

			static BinomialTree<Value> MakeTian( double mu, double sigma, double delta ) {
				double a = std::exp( mu*delta );
				double b = std::exp( sigma*sigma * delta );
				double c = 0.5 * a * b;
				double s = std::sqrt( b*b + 2.0*b - 3.0 );
				double up = c*( b + 1.0 + s );
				double down = c*( b + 1.0 - s );
				return BinomialTree<Value>( up, down, (a-down)/(up-down) );
			}

			static BinomialTree<Value> MakeTrigeorgis( double mu, double sigma, double delta ) {
				double sigma2 = sigma*sigma;
				double gamma = mu - 0.5*sigma2;
				double x = std::sqrt(sigma2*delta+gamma*gamma*delta*delta);
				double up = std::exp(x);
				return BinomialTree<Value>( up, 1.0/up, 0.5 * ( 1.0 + gamma*delta/x ) );
			}

		};
	}

}

#endif
