#include <mqf/processes/gbm.h>
#include <mqf/processes/ornstein_uhlenbeck.h>
#include <mqf/processes/binomial_tree.h>
#include <mqf/processes/trinomial_tree.h>
#include <mqf/sde/milstein.h>
#include <mqf/regression/least_squares.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace mqf;

int main() {

	auto tree = Processes::TrinomialTree<>::MakeBoyle( 0.1, 0.3, 1.0/360 );
	tree.writeStateTree( 1.0, 30, "trinomial.csv" );
	tree.writeProbabilityTree( 30, "trinomialp.csv" );
	return 0;

	auto model = Processes::TrinomialTree<>::MakeBoyle(0.1,0.3,1.0/360);

	Milstein<decltype(model)> milstein(model);

	double t = 0.0,
		   tMax = 1.0,
	       dt = 1.0/360;

	double x = 1.0;

	cout << model.up << endl;
	cout << model.down << endl;

	for( auto p : model.dist.param().probabilities() )
		cout << p << endl;

	ofstream out("test.csv");

	
	while( t < tMax ) {
		out << t << "," << x << endl;
		x = model.advance(x);
		t += dt;
	}

	cout << "Press enter to continue . . . "; cin.get();
}

