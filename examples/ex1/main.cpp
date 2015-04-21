#include <mqf/sde/milstein.h>
#include <mqf/processes/gbm.h>
#include <mqf/processes/ornstein_uhlenbeck.h>
#include <mqf/utility.h>
#include <mqf/functions.h>
#include <iostream>
#include <fstream>
#include <mqf/trading/momentum.h>

using namespace std;
using namespace mqf;

int main() {

	double drift = 0.2;
	double vol = 1.5;
	auto model = Processes::GBM<>(drift+0.5*vol*vol,vol);

	Milstein<decltype(model)> milstein(model);

	double t = 0.0,
	       tMax = 3.0,
	       dt = 1.0/252;

	double x = 1.0;

	ofstream out("sde.csv");
	double portfolio = 0.0;
	double investment = 10000.0;
	double cash = investment;
	MAStrategy strat;
	std::vector<double> timeseries;
	timeseries.reserve( 1000 );
	double volume = investment * 10;
	double total;
	while( t < tMax ) {
		timeseries.push_back(x);
		
		auto action = strat.compute( timeseries.begin(), timeseries.end() );
		if( action.type == Action::Buy ) {
			double buy = std::fmin( x * volume, cash ) / x;
			cash -= x * buy;
			portfolio += buy;
		}
		if( action.type == Action::Sell ) {
			cash += x * portfolio;
			portfolio = 0;
		}

		total = cash + portfolio * x;

		//double bench = logReturn(1.0,x);
		double bench = 0.13 * t;
		double perf = logReturn(investment,total);
		double diff = perf - bench;
		out << t << "," << x << "," << portfolio << "," << cash << "," << bench << "," << perf << "," << diff << endl;

		x = milstein.advance( x, dt );
		t += dt;
	}

	cout << "Press enter to continue . . . "; cin.get();
}
