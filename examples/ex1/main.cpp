#include <mqf/sde/milstein.h>
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
#include <mqf/chrono.h>
#include <mqf/fibonacci.h>

using namespace std;
using namespace mqf;

int main() {

	double drift = 0.00;
	double vol = 0.05;
	auto model = Processes::GBM<>(drift+0.5*vol*vol,vol);

	Milstein<decltype(model)> milstein(model);

	double t = 0.0,
	       tMax = 3.0,
	       dt = 1.0/252;

	double x = 1.0;
	for(int i=0;i<0;++i)
		milstein.advance( x, dt );
	
	std::vector<double> timeseries;
	timeseries.reserve( 1 + (tMax-t) / dt );
	while( t < tMax ) {
		timeseries.push_back(x);
		//milstein.sde.mu += dt;
		x = milstein.advance( x, dt );
		t += dt;
	}

	{
		CW1 strat;
		Backtest<CW1> bt(strat);
		bt.runTest( "strat-1.csv", timeseries.begin(), timeseries.end() );
	}
	{
		Stripes strat;
		Backtest<Stripes> bt(strat);
		bt.runTest( "strat-stripes.csv", timeseries.begin(), timeseries.end() );
	}
	{
		MAStrategy strat;
		Backtest<MAStrategy> bt(strat);
		bt.runTest( "strat-ma.csv", timeseries.begin(), timeseries.end() );
	}

	cout << "Press enter to continue . . . "; cin.get();
}
