#include <mqf/sde/milstein.h>
#include <mqf/processes/gbm.h>
#include <mqf/utility.h>
#include <iostream>
#include <fstream>
#include <mqf/time_series/moving_average.h>

using namespace std;
using namespace mqf;

struct Action {
	enum actionType { Hold, Buy, Sell } type;
	double amount;

	explicit Action( actionType type = Hold, double amount = 0.0 ) : type(type), amount(amount) {}
};

struct Strategy1 {
	double sellLimit, buyLimit;

	explicit Strategy1( double sellLimit = 0.0,
	                    double buyLimit = 1.0 ) :
		sellLimit(sellLimit),
		buyLimit(buyLimit)
	{}

	template<typename It>
	Action compute( It p1, It p2 ) const {
		double mu, sigma2;
		double h = (mu - 0.5 * sigma2) / sigma2;
		if( h > buyLimit ) {
			return Action( Action::Buy );
		}
		if( h < sellLimit ) {
			return Action( Action::Sell );
		}
		return Action( Action::Hold );
	}

};

struct MAStrategy {
	int shortPeriod, longPeriod;

	explicit MAStrategy( int shortPeriod = 50,
	                     int longPeriod = 200 ) :
		shortPeriod(shortPeriod),
		longPeriod(longPeriod)
	{}

	template<typename It>
	Action compute( It p1, It p2 ) const {
		
		auto shortMA = SimpleMovingAverage(shortPeriod).compute(p1,p2);
		auto longMA = SimpleMovingAverage(longPeriod).compute(p1,p2);
		
		if( shortMA.size() == 0 || longMA.size() == 0 )
			return Action( Action::Hold );

		if( shortMA.back() > longMA.back() ) {
			return Action( Action::Buy );
		}
		if( shortMA.back() < longMA.back() ) {
			return Action( Action::Sell );
		}
		return Action( Action::Hold );
	}

};

struct SimpleStrategy {

	template<typename It>
	Action compute( It p1, It p2 ) const {
		if( p1 == p2 ) return Action();
		auto current = *--p2;
		if( p1 == p2 ) return Action();
		auto previous = *--p2;
		if( current > previous )
			return Action( Action::Buy );
		if( current < previous )
			return Action( Action::Sell );
		return Action( Action::Hold );
	}

};

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
