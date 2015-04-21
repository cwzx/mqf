#include <mqf/sde/milstein.h>
#include <mqf/processes/gbm.h>
#include <mqf/utility.h>
#include <mqf/functions.h>
#include <iostream>
#include <fstream>
#include <mqf/time_series/moving_average.h>

using namespace std;
using namespace mqf;

int main() {

	auto model = Processes::GBM<>(0.12,0.5);

	Milstein<decltype(model)> milstein(model);

	double t = 0.0,
	       tMax = 50.0,
	       dt = 1.0/252;

	double x = 1.0;

	ofstream out("sde.csv");
	double portfolio = 0.0;
	double cash = 0.0;
	SimpleStrategy strat1;
	MAStrategy strat2(7,60);
	std::vector<double> timeseries;
	timeseries.reserve( 10000 );
	double volume = 1.0;

	while( t < tMax ) {
		timeseries.push_back(x);
		
		auto action = strat1.compute( timeseries.begin(), timeseries.end() );
		if( action.type == Action::Buy ) {
			cash -= x * volume;
			portfolio += volume;
		}
		if( action.type == Action::Sell ) {
			cash += x * portfolio;
			portfolio = 0;
		}

		double total = cash + portfolio * x;
		out << t << "," << x << "," << portfolio << "," << cash << "," << total << endl;
		x = milstein.advance( x, dt );
		t += dt;
	}

	cout << "Press enter to continue . . . "; cin.get();
}
