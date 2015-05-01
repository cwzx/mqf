#ifndef INCLUDED_MQF_BACKTEST
#define INCLUDED_MQF_BACKTEST
#include "../stats/descriptive.h"
#include "../sde/milstein.h"
#include "action.h"
#include <cstdint>
#include "return.h"
#include "../time_series/drawdown.h"

namespace mqf {

	struct TestResult {
		double annualReturn,
		       annualVolatility,
		       sharpeRatio;
		DrawDown maxDrawDown;
	};

	template<typename Strat>
	struct Backtest {
		Strat strategy;

		explicit Backtest( const Strat& s ) : strategy(s) {}

		template<typename It>
		TestResult runTest( const char* file, It p1, It p2 ) {

			TestResult result = {};
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

				double stock = logReturn(inital_stock,x);
				double bench = 0.13 * t;
				double perf = logReturn(investment,total);
				double diff = perf - bench;
				out << t << "," << stock << "," << x << "," << action.amount << "," << portfolio << "," << cash << "," << perf << "," << diff << endl;
				t += dt;
			}
			return result;
		}
	};

	template<typename Strat,typename SDE>
	struct StochasticBacktest {
		double tMax = 1.0;
		double dt = 1.0 / 252;
		double initial = 1.0;

		uint32_t repeats = 1000;

		StochasticBacktest() = default;
		StochasticBacktest( const Strat& strat, const SDE& sde ) : strategy(strat), dynamics(sde) {}

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

					if( action.type == Action::Buy ) {
						double buy = std::fmin( cash /** action.amount*/, cash ) / x;
						cash -= x * buy;
						portfolio += buy;
					}
					if( action.type == Action::Sell ) {
						cash += x * portfolio;
						portfolio = 0;
					}
					x = dynamics.advance( x, dt );
					t += dt;
				}

				double ar = logReturn( investment, cash + portfolio * prices.back() );
				double sr = logReturn( initial, prices.back() );
				algoReturn += ar;
				stockReturn += sr;
				out << sr << "," << ar << endl;
			}

			algoReturn /= repeats;
			stockReturn /= repeats;
			//out << stockReturn << "," << algoReturn << endl;

		}

	};

	inline double sharpeRatio( double mean, double vol, double riskFreeRate ) {
		return ( mean - riskFreeRate ) / vol;
	}

	template<typename It>
	TestResult computeTestResults( It p1, It p2, double dt, double riskFreeRate = 0.0 ) {
		auto returns = computeLogReturns( p1, p2 );
		auto mean = sampleMean( returns );
		auto var = sampleVariance( returns.begin(), returns.end(), mean );
		auto annualLogReturn = mean / dt;
		auto annualVar = var / dt;

		TestResult res;
		res.annualReturn = returnFromLogReturn( annualLogReturn );
		res.annualVolatility = std::sqrt( annualVar );
		res.sharpeRatio = sharpeRatio( res.annualReturn, res.annualVolatility, riskFreeRate );
		res.maxDrawDown = maxDrawDown(p1,p2);
		return res;
	}

}

#endif
