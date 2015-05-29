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
#include <mqf/digamma.h>
#include <mqf/trigamma.h>
#include <mqf/stats/histogram.h>
#include <mqf/stats/mle.h>
#include <mqf/distributions/cauchy.h>
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
		auto normal = MLE<Normal>()( ret.begin(), ret.end() );
		plot(("returns-histn-" + ticker + ".csv").c_str(),-1.0,1.0,1000,normal);
		auto mu = sampleMean(ret.begin(), ret.end());
		auto sigma = sampleStdDev( ret.begin(), ret.end(), mu );
		Cauchy cauchy( mu, 0.5 * sigma );
		plot(("returns-histc-" + ticker + ".csv").c_str(),-1.0,1.0,1000,cauchy);
		auto k = sampleExKurtosis( ret.begin(), ret.end(), mu );
		auto nu = 6.0 / k + 4.0;
		cout << ticker << " nu = " << nu << "\n";
		StudentsTLS student( nu, mu, sigma*sigma * (nu - 2.0)/nu );
		plot(("returns-histt-" + ticker + ".csv").c_str(),-1.0,1.0,1000,student);
	}
	/*{
		DifferentialEvolution<double,2> de;
		de.bounds.minBounds[0] = -0.2;
		de.bounds.maxBounds[0] = 0.2;
		de.bounds.minBounds[1] = 1;
		de.bounds.maxBounds[1] = 200;
		de.max_evals = 500;

		auto cost = [&](auto&& x) {
			CW1 strat(x[0],(int)x[1]);
			Backtest<CW1> bt(strat);
			auto res = bt.runTest( timeseries.begin(), timeseries.end() );
			return -res.annualLogReturn;
		};

		auto r = de.optimize( cost );

		cout << "opt: " << r << endl;
		
		CW1 strat(r[0],(int)r[1]);
		Backtest<CW1> bt(strat);
		auto res = bt.runTest( ("strat-1-" + ticker + ".csv").c_str(), timeseries.begin(), timeseries.end() );
		res.print();
		ofstream out("params-1-" + ticker + ".txt");
		out << r << endl;
		res.print( out );
	}
	{
		DifferentialEvolution<double,2> de;
		de.bounds.minBounds[0] = 1;
		de.bounds.maxBounds[0] = 50;
		de.bounds.minBounds[1] = 1;
		de.bounds.maxBounds[1] = 200;
		de.max_evals = 10000;
		
		auto cost = [&](auto&& x) {
			BasicMA strat( (int)x[0], (int)x[1] );
			Backtest<BasicMA> bt(strat);
			auto res = bt.runTest( timeseries.begin(), timeseries.end() );
			return -res.annualLogReturn;
		};
		
		auto r = de.optimize( cost );

		cout << "opt: " << r << endl;

		BasicMA strat((int)r[0],(int)r[1]);
		Backtest<BasicMA> bt(strat);
		auto res = bt.runTest( ("strat-ma-" + ticker + ".csv").c_str(), timeseries.begin(), timeseries.end() );
		res.print();
		ofstream out("params-ma-" + ticker + ".txt");
		out << r << endl;
		res.print( out );
	}*/
	/*{
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
		bf.grid.size[0] = 50;
		bf.grid.size[1] = 100;
		bf.bounds.minBounds[0] = 1;
		bf.bounds.maxBounds[0] = 50;
		bf.bounds.minBounds[1] = 1;
		bf.bounds.maxBounds[1] = 100;
		
		auto cost = [&](auto&& x) {
			BasicMA strat( (int)x[0], (int)x[1] );
			Backtest<BasicMA> bt(strat);
			auto res = bt.runTest( timeseries.begin(), timeseries.end() );
			return -res.sharpeRatio;
		};
		
		auto r = bf.optimize( cost );

		r.histogram.writeCSV( ("h-ma-" + ticker + ".csv").c_str() );

		cout << "opt: " << r.optimal << endl;

		BasicMA strat((int)r.optimal[0],(int)r.optimal[1]);
		Backtest<BasicMA> bt(strat);
		auto res = bt.runTest( ("strat-ma-" + ticker + ".csv").c_str(), timeseries.begin(), timeseries.end() );
		res.print();
		ofstream out("params-ma-" + ticker + ".txt");
		out << r.optimal << endl;
		res.print( out );
	}*/

}

int main() {

	const char* tickers[] = { "AAPL", "IBM", "AMZN", "NFLX", "GOOGL", "MSFT" };

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
