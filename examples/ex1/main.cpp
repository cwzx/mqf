#include <mqf/processes/gbm.h>
#include <mqf/processes/ornstein_uhlenbeck.h>
#include <mqf/utility.h>
#include <mqf/functions.h>
#include <iostream>
#include <fstream>
#include <mqf/trading/strategies/cw1.h>
#include <mqf/trading/strategies/moving_average.h>
#include <mqf/trading/strategies/stripes.h>
#include <mqf/trading/backtest.h>

using namespace std;
using namespace mqf;

int main() {

	double drift = 0.1;
	double vol = 0.1;
	auto model = Processes::GBM<>(drift+0.5*vol*vol,vol);

	std::vector<double> timeseries;
	timeseries.reserve( 10000 );

	ifstream in("ftse.txt");
	while( !in.eof() ) {
		double x = 0.0;
		in >> x;
		timeseries.push_back( x );
	}
	std::reverse( timeseries.begin(), timeseries.end() );

	{
		CW1 strat;
		StochasticBacktest<CW1,decltype(model)> bt(strat,model);
		bt.repeats = 100000;
		bt.run( "strat-1.csv" );
	}
	{
		MAStrategy strat;
		StochasticBacktest<MAStrategy,decltype(model)> bt(strat,model);
		bt.repeats = 100000;
		bt.run( "strat-ma.csv" );
	}
	/*{
		CW1 strat;
		Backtest<CW1> bt(strat);
		bt.runTest( "strat-1.csv", timeseries.begin(), timeseries.end() );
	}
	{
		MAStrategy strat;
		Backtest<MAStrategy> bt(strat);
		bt.runTest( "strat-ma.csv", timeseries.begin(), timeseries.end() );
	}*/
	/*{
		Stripes strat;
		Backtest<Stripes> bt(strat);
		bt.runTest( "strat-stripes.csv", timeseries.begin(), timeseries.end() );
	}*/
	

	cout << "Press enter to continue . . . "; cin.get();
}
