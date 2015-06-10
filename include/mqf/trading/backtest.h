#ifndef INCLUDED_MQF_BACKTEST
#define INCLUDED_MQF_BACKTEST
#include <cstdint>
#include "results.h"
#include "action.h"
#include "../finance/interest.h"
#include "../gregorian.h"

namespace mqf {

	struct BacktestSpec {
		Gregorian::Date start, end;

		double riskFreeRate;

		std::vector<double> benchmark;
	
	};

	template<typename Strat>
	struct Backtest {
		Strat strategy;

		explicit Backtest( const Strat& s ) : strategy(s) {}

		template<typename It>
		TestResult run( const char* file, It p1, It p2 ) {

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
		
				Action action = strategy.compute( p1, std::next(q) );
				if( action.type == Action::Buy ) {
					double buy = std::fmin( cash * action.amount, cash ) / x;
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
				double bench = stock;
				double perf = logReturn(investment,total);
				double diff = perf - bench;
				out << t << ',' << stock << ',' << x << ',' << action.amount << ',' << portfolio << ',' << cash << ',' << perf << ',' << diff << '\n';
				t += dt;
			}

			return computeTestResults( totals.begin(), totals.end(), dt );
		}

		template<typename It>
		TestResult run( It p1, It p2 ) {

			std::vector<double> totals;
			totals.reserve( std::distance(p1,p2) );
			double portfolio = 0.0;
			double cash = 10000.0;
			double t = 0, dt = 1.0/252;
			for(auto q = p1; q != p2; ++q) {

				double x = *q;

				Action action = strategy.compute( p1, std::next(q) );
				if( action.type == Action::Buy ) {
					double buy = std::fmin( cash * action.amount, cash ) / x;
					cash -= x * buy;
					portfolio += buy;
				}
				if( action.type == Action::Sell ) {
					cash += x * portfolio;
					portfolio = 0;
				}

				double total = cash + portfolio * x;
				totals.push_back( total );

				t += dt;
			}

			return computeTestResults( totals.begin(), totals.end(), dt );
		}
	};

}

#endif
