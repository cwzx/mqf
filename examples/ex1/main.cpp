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

using namespace std;
using namespace mqf;

int main() {

	double drift = 0.1;
	double vol = 0.1;
	auto model = Processes::GBM<>( drift + 0.5*vol*vol, vol );

	string ticker = "aapl";

	auto data = Yahoo::loadWithSplits( (ticker + ".csv").c_str() );

	auto start = std::find_if( data.begin(), data.end(), [](auto&& x){ return x.date.year >= 1900; } );
	auto end   = std::find_if(        start, data.end(), [](auto&& x){ return x.date.year >= 2100; } );

	std::vector<double> timeseries;
	timeseries.reserve( data.size() );
	std::transform( start,
					end,
					std::back_inserter(timeseries),
					[](auto&& x){ return x.close; } );

	/*{
		CW1 strat;
		StochasticTest<CW1,decltype(model)> bt(strat,model);
		bt.repeats = 100000;
		bt.run( "strat-1.csv" );
	}
	{
		MAStrategy strat;
		StochasticTest<MAStrategy,decltype(model)> bt(strat,model);
		bt.repeats = 100000;
		bt.run( "strat-ma.csv" );
	}*/
	{
		CW1 strat;
		Backtest<CW1> bt(strat);
		auto res = bt.runTest( ("strat-1-" + ticker + ".csv").c_str(), timeseries.begin(), timeseries.end() );
		res.print();
	}
	{
		MAStrategy strat;
		Backtest<MAStrategy> bt(strat);
		//auto res = bt.runTest( ("strat-ma-" + ticker + ".csv").c_str(), timeseries.begin(), timeseries.end() );
		//res.print();
	}

	cout << "Press enter to continue . . . "; cin.get();
}
