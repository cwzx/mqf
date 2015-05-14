#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <mqf/processes/gbm.h>
#include <mqf/processes/ornstein_uhlenbeck.h>
#include <mqf/utility.h>
#include <mqf/functions.h>
#include <mqf/trading/strategies/cw1.h>
#include <mqf/trading/strategies/moving_average.h>
#include <mqf/trading/strategies/stripes.h>
#include <mqf/trading/backtest.h>
#include <mqf/data/yahoo.h>
#include <mqf/optimization/brute_force.h>

using namespace std;
using namespace mqf;

void test( const string& ticker ) {

	auto data = Yahoo::load( (ticker + ".csv").c_str() );

	auto timeseries = data.computeAdjustedClose();

	{
		BruteForce<double,2> bf;
		bf.grid.size[0] = 21;
		bf.grid.size[1] = 61;
		bf.bounds.minBounds[0] = 0.0;
		bf.bounds.maxBounds[0] = 0.1;
		bf.bounds.minBounds[1] = 20;
		bf.bounds.maxBounds[1] = 200;

		auto cost = [&](auto&& x) { 
			CW1 strat(x[0],(int)x[1]);
			Backtest<CW1> bt(strat);
			auto res = bt.runTest( timeseries.begin(), timeseries.end() );
			return -res.sharpeRatio;
		};

		auto r = bf.optimize( cost );

		r.histogram.writeCSV(("h-1-" + ticker + ".csv").c_str());

		cout << "opt: " << r.optimal << endl;
		
		CW1 strat(r.optimal[0],(int)r.optimal[1]);
		Backtest<CW1> bt(strat);
		auto res = bt.runTest( ("strat-1-" + ticker + ".csv").c_str(), timeseries.begin(), timeseries.end() );
		res.print();
		ofstream out("params-1-" + ticker + ".txt");
		out << r.optimal << endl;
		res.print( out );
	}
	{
		BruteForce<double,2> bf;
		bf.grid.size[0] = 25;
		bf.grid.size[1] = 88;
		bf.bounds.minBounds[0] = 1;
		bf.bounds.maxBounds[0] = 25;
		bf.bounds.minBounds[1] = 26;
		bf.bounds.maxBounds[1] = 200;
		
		auto cost = [&](auto&& x) {
			MAStrategy strat( (int)x[0], (int)x[1] );
			Backtest<MAStrategy> bt(strat);
			auto res = bt.runTest( timeseries.begin(), timeseries.end() );
			return -res.sharpeRatio;
		};
		
		auto r = bf.optimize( cost );

		r.histogram.writeCSV(("h-ma-" + ticker + ".csv").c_str());

		cout << "opt: " << r.optimal << endl;

		MAStrategy strat((int)r.optimal[0],(int)r.optimal[1]);
		Backtest<MAStrategy> bt(strat);
		auto res = bt.runTest( ("strat-ma-" + ticker + ".csv").c_str(), timeseries.begin(), timeseries.end() );
		res.print();
		ofstream out("params-ma-" + ticker + ".txt");
		out << r.optimal << endl;
		res.print( out );
	}

}

int main() {

	const char* tickers[] = { "aapl", "ibm", "amzn", "nflx", "googl", "msft" };

	for( auto&& ticker : tickers )
		test( ticker );

	auto bench = Yahoo::load( "SPY.csv" );

	auto adjclose = bench.computeAdjustedClose();

	auto result = computeTestResults( adjclose.begin(), adjclose.end(), 1.0/252 );
	result.print();

	auto returns = computeCumulativeLogReturns( adjclose.begin(), adjclose.end() );
	ofstream out("bench.csv");
	for( auto x : returns )
		out << x << endl;


	cout << "Press enter to continue . . . "; cin.get();
}
