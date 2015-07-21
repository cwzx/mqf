#ifndef INCLUDED_MQF_STOCHASTIC_TEST
#define INCLUDED_MQF_STOCHASTIC_TEST
#include "results.h"
#include "../sde/milstein.h"
#include "action.h"
#include <cstdint>

namespace mqf {

	template<typename Strat,typename SDE>
	struct StochasticTest {
		double tMax = 1.0;
		double dt = 1.0 / 252;
		double initial = 1.0;

		uint32_t repeats = 1000;

		StochasticTest() = default;
		StochasticTest( const Strat& strat, const SDE& sde ) : strategy(strat), dynamics(sde) {}

		Milstein<SDE> dynamics;
		Strat strategy;

		void run( const char* file ) {

			ofstream out(file);
			std::vector<double> prices;
			prices.reserve( 1 + tMax / dt );

			double algoReturn = 0.0;
			double stockReturn = 0.0;

			for(uint32_t i=0;i<repeats;++i) {
				double x = initial;
				double t = 0.0;
				double portfolio = 0.0;
				double investment = 10000.0;
				double cash = investment;
				prices.clear();
				while( t < tMax ) {
					prices.push_back(x);

					auto action = strategy.compute( prices.begin(), prices.end() );
					cash += x * portfolio;
					portfolio = 0;

					if( action.type == Action::Long ) {
						double buy = std::fmax( cash, 0.0 ) / x;
						cash -= x * buy;
						portfolio += buy;
					}
				
					if( action.type == Action::Short ) {
						double sell = std::fmax( cash, 0.0 ) / x;
						cash += x * sell;
						portfolio -= sell;
					}
					x = dynamics.advance( x, dt );
					t += dt;
				}

				double ar = logReturn( investment, cash + portfolio * prices.back() );
				double sr = logReturn( initial, prices.back() );
				algoReturn += ar;
				stockReturn += sr;
				out << sr << ',' << ar << endl;
			}

			algoReturn /= repeats;
			stockReturn /= repeats;
			//out << stockReturn << ',' << algoReturn << endl;

		}

	};

}

#endif
