#ifndef INCLUDED_MQF_DISTRIBUTIONS_MAXWELL_BOLTZMANN
#define INCLUDED_MQF_DISTRIBUTIONS_MAXWELL_BOLTZMANN
#include <cmath>
#include "../distribution.h"
#include "../random_variable.h"
#include "../constants.h"

namespace mqf {
namespace Distributions {

	struct MaxwellBoltzmann : Density<> {
		double a;
		
		explicit MaxwellBoltzmann( double a = 1.0 ) : a(a) {}

		double mean() const {
			return a * std::sqrt(8.0/Pi);
		}

		double mode() const {
			return a * std::sqrt(2.0);
		}

		double variance() const {
			return a*a * (3.0*Pi - 8.0) / Pi;
		}

		double operator()( Value x ) const {
			double y = x / a;
			return std::sqrt(2.0/Pi) * y*y * std::exp(-0.5*y*y) / a;
		}

		double derivative( Value x ) const {
			double y = x / a;
			return std::sqrt(2.0/Pi) * y * std::exp(-0.5*y*y) * ( 2.0 - y*y ) / (a*a);
		}

		double cumulative( Value x ) const {
			double y = x / a;
			return std::erf( y / std::sqrt(2.0) ) - std::sqrt(2.0/Pi) * y * std::exp(-0.5*y*y);
		}

	};

}
}

#endif
