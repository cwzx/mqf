#include <mqf/sde/milstein.h>
#include <mqf/processes/gbm.h>
#include <mqf/processes/ornstein_uhlenbeck.h>
#include <mqf/utility.h>
#include <mqf/functions.h>
#include <iostream>
#include <fstream>
#include <mqf/trading/momentum.h>
#include <mqf/trading/backtest.h>

using namespace std;
using namespace mqf;

int main() {

	double drift = 0.25;
	double vol = 0.5;
	auto model = Processes::GBM<>(drift+0.5*vol*vol,vol);

	Milstein<decltype(model)> milstein(model);

	double t = 0.0,
	       tMax = 3.0,
	       dt = 1.0/252;

	double x = 1.0;

	
	std::vector<double> timeseries;
	timeseries.reserve( 1000 );
	while( t < tMax ) {
		timeseries.push_back(x);
		x = milstein.advance( x, dt );
		t += dt;
	}

	Strategy1 strat;
	Backtest<Strategy1> bt(strat);
	bt.runTest( timeseries.begin(), timeseries.end() );

	cout << "Press enter to continue . . . "; cin.get();
}
