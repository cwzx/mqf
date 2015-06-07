#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <mqf/utility.h>
#include <mqf/functions.h>
#include <mqf/trading/strategies/cw1.h>
#include <mqf/trading/strategies/moving_average.h>
#include <mqf/trading/backtest.h>
#include <mqf/data/yahoo.h>
#include <mqf/optimization/differential_evolution.h>
#include <mqf/optimization/brute_force.h>
#include <mqf/digamma.h>
#include <mqf/trigamma.h>
#include <mqf/stats/histogram.h>
#include <mqf/stats/mle.h>
#include <mqf/distributions/students_t.h>

using namespace std;
using namespace mqf;
using namespace mqf::Strategies;
using namespace mqf::Distributions;

void test( const string& ticker ) {

	auto data = Yahoo::load( (ticker + ".csv").c_str() );

	auto timeseries = data.computeAdjustedClose();
	{
		auto ret = computeLogReturns( timeseries.begin(), timeseries.end() );

		HistogramGenerator().generate( ret.begin(), ret.end() ).writeCSV( ("returns-" + ticker + ".csv").c_str() );

		auto student = MomentEstimation<StudentsTLS>()( ret.begin(), ret.end() );
		plot(("returns-histt-" + ticker + ".csv").c_str(),-0.5,0.5,1000,student);
		cout << ticker << " nu = " << student.nu << " mu = " << student.mu << " sigma2 = " << student.sigma2 << '\n';

	}
	/*{
		CW1 strat;
		Backtest<CW1> bt(strat);
		auto res = bt.runTest( ("strat-1-" + ticker + ".csv").c_str(), timeseries.begin(), timeseries.end() );
		res.print();
		ofstream out("params-1-" + ticker + ".txt");
		res.print( out );
	}
	{

		BasicMA strat;
		Backtest<BasicMA> bt(strat);
		auto res = bt.runTest( ("strat-ma-" + ticker + ".csv").c_str(), timeseries.begin(), timeseries.end() );
		res.print();
		ofstream out("params-ma-" + ticker + ".txt");
		res.print( out );
	}*/
	{
		DifferentialEvolution<double,2> de;
		de.bounds.lower[0] = 0.0;
		de.bounds.upper[0] = 0.3;
		de.bounds.lower[1] = 10;
		de.bounds.upper[1] = 200;
		de.max_evals = 500;

		auto cost = [&]( auto&& x ) {
			CW1 strat(x[0],(int)x[1]);
			Backtest<CW1> bt(strat);
			auto res = bt.runTest( timeseries.begin(), timeseries.end() );
			return -res.sharpeRatio;
		};

		auto r = de.optimize( cost );

		cout << "opt: " << r << '\n';
		
		CW1 strat(r[0],(int)r[1]);
		Backtest<CW1> bt(strat);
		auto res = bt.runTest( ("strat-1-" + ticker + ".csv").c_str(), timeseries.begin(), timeseries.end() );
		res.print();
		ofstream out("params-1-" + ticker + ".txt");
		out << r << '\n';
		res.print( out );
	}
	{
		DifferentialEvolution<double,2> de;
		de.bounds.lower[0] = 1;
		de.bounds.upper[0] = 30;
		de.bounds.lower[1] = 30;
		de.bounds.upper[1] = 200;
		de.max_evals = 10000;
		
		auto cost = [&]( auto&& x ) {
			BasicMA strat( (int)x[0], (int)x[1] );
			Backtest<BasicMA> bt(strat);
			auto res = bt.runTest( timeseries.begin(), timeseries.end() );
			return -res.sharpeRatio;
		};
		
		auto r = de.optimize( cost );

		cout << "opt: " << r << '\n';

		BasicMA strat((int)r[0],(int)r[1]);
		Backtest<BasicMA> bt(strat);
		auto res = bt.runTest( ("strat-ma-" + ticker + ".csv").c_str(), timeseries.begin(), timeseries.end() );
		res.print();
		ofstream out("params-ma-" + ticker + ".txt");
		out << r << '\n';
		res.print( out );
	}
	/*{
		BruteForce<double,2> bf;
		bf.grid.size[0] = 21;
		bf.grid.size[1] = 61;
		bf.bounds.lower[0] = 0.0;
		bf.bounds.upper[0] = 0.25;
		bf.bounds.lower[1] = 20;
		bf.bounds.upper[1] = 200;

		auto cost = [&](auto&& x) { 
			CW1 strat(x[0],(int)x[1]);
			Backtest<CW1> bt(strat);
			auto res = bt.runTest( timeseries.begin(), timeseries.end() );
			return -res.annualLogReturn;
		};

		auto r = bf.optimize( cost );

		r.histogram.writeCSV(("h-1-" + ticker + ".csv").c_str());

		cout << "opt: " << r.optimal << '\n';
		
		CW1 strat(r.optimal[0],(int)r.optimal[1]);
		Backtest<CW1> bt(strat);
		auto res = bt.runTest( ("strat-1-" + ticker + ".csv").c_str(), timeseries.begin(), timeseries.end() );
		res.print();
		ofstream out("params-1-" + ticker + ".txt");
		out << r.optimal << '\n';
		res.print( out );
	}
	{
		BruteForce<double,2> bf;
		bf.grid.size[0] = 50;
		bf.grid.size[1] = 100;
		bf.bounds.lower[0] = 1;
		bf.bounds.upper[0] = 50;
		bf.bounds.lower[1] = 1;
		bf.bounds.upper[1] = 200;
		
		auto cost = [&](auto&& x) {
			BasicMA strat( (int)x[0], (int)x[1] );
			Backtest<BasicMA> bt(strat);
			auto res = bt.runTest( timeseries.begin(), timeseries.end() );
			return -res.annualLogReturn;
		};
		
		auto r = bf.optimize( cost );

		r.histogram.writeCSV( ("h-ma-" + ticker + ".csv").c_str() );

		cout << "opt: " << r.optimal << '\n';

		BasicMA strat((int)r.optimal[0],(int)r.optimal[1]);
		Backtest<BasicMA> bt(strat);
		auto res = bt.runTest( ("strat-ma-" + ticker + ".csv").c_str(), timeseries.begin(), timeseries.end() );
		res.print();
		ofstream out("params-ma-" + ticker + ".txt");
		out << r.optimal << '\n';
		res.print( out );
	}*/

}

int main() {
	
	/*test( "SPY" );
	return 0;*/

	const char* tickers[] = {
		"AAPL",
		"MSFT",
		"GOOGL",
		"GOOG",
		"FB",
		"AMZN",
		"GILD",
		"CMCSA",
		"INTC",
		"CMCSK",
		"CSCO",
		"AMGN",
		"QCOM",
		"CELG",
		"BIIB",
		"VOD",
		"WBA",
		"SBUX",
		"BIDU",
		"AMOV",
		"SPY"
	};

	for( auto&& ticker : tickers )
		test( ticker );

	auto bench = Yahoo::load( "SPY.csv" );

	auto adjclose = bench.computeAdjustedClose();

	auto result = computeTestResults( adjclose.begin(), adjclose.end(), 1.0/252 );
	result.print();

	auto returns = computeCumulativeLogReturns( adjclose.begin(), adjclose.end() );
	ofstream out("bench.csv");
	for( auto x : returns )
		out << x << '\n';


	cout << "Press enter to continue . . . "; cin.get();
}
