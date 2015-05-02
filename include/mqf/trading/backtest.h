#ifndef INCLUDED_MQF_BACKTEST
#define INCLUDED_MQF_BACKTEST
#include "results.h"
#include "action.h"
#include <cstdint>
#include "../stats/histogram.h"

namespace mqf {

	template<typename Strat>
	struct Backtest {
		Strat strategy;

		explicit Backtest( const Strat& s ) : strategy(s) {}

		template<typename It>
		TestResult runTest( const char* file, It p1, It p2 ) {

			std::vector<double> totals;
			totals.reserve( std::distance(p1,p2) );
			ofstream out(file);
			double portfolio = 0.0;
			double investment = 10000.0;
			double cash = investment;
			double t = 0, dt = 1.0/252;
			double inital_stock = *p1;
			for(auto q = p1; q != p2; ++q) {

				double x = *q;
		
				auto action = strategy.compute( p1, std::next(q) );
				if( action.type == Action::Buy ) {
					double buy = std::fmin( cash /** action.amount*/, cash ) / x;
					cash -= x * buy;
					portfolio += buy;
				}
				if( action.type == Action::Sell ) {
					cash += x * portfolio;
					portfolio = 0;
				}

				double total = cash + portfolio * x;
				totals.push_back( total );

				double stock = logReturn(inital_stock,x);
				double bench = 0.13 * t;
				double perf = logReturn(investment,total);
				double diff = perf - bench;
				out << t << "," << stock << "," << x << "," << action.amount << "," << portfolio << "," << cash << "," << perf << "," << diff << endl;
				t += dt;
			}

			return computeTestResults( totals.begin(), totals.end(), dt );
		}
	};

}

#endif
