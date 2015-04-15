#include <mqf/sde/milstein.h>
#include <mqf/processes/square_root.h>
#include <mqf/processes/exponential_vasicek.h>
#include <mqf/finance/fixed_coupon_bond.h>
#include <mqf/finance/floating_rate_note.h>
#include <mqf/finance/zcb.h>
#include <iostream>
#include <fstream>

using namespace std;
using namespace mqf;

int main() {

	FixedCouponBond<> fcb;
	FRN<10> frn;
	ZCB<> zcb;

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

