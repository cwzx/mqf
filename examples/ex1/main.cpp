#include <mqf/processes/gbm.h>
#include <mqf/processes/ornstein_uhlenbeck.h>
#include <mqf/processes/binomial_tree.h>
#include <mqf/processes/trinomial_tree.h>
#include <mqf/sde/milstein.h>
#include <mqf/regression/least_squares.h>
#include <mqf/processes/square_root.h>
#include <mqf/processes/exponential_vasicek.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace mqf;

int main() {

	auto model = Processes::ExponentialVasicek(std::log(0.1),1.0,0.3);

	Milstein<decltype(model)> milstein(model);

	double t = 0.0,
	       tMax = 10.0,
	       dt = 1.0/360;

	double x = 0.1;

	ofstream out("test.csv");

	while( t < tMax ) {
		out << t << "," << x << endl;
		x = milstein.advance( x, dt );
		t += dt;
	}

	cout << "Press enter to continue . . . "; cin.get();
}

