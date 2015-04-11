#ifndef INCLUDED_MQF_DISTRIBUTIONS_RAYLEIGH
#define INCLUDED_MQF_DISTRIBUTIONS_RAYLEIGH
#include "../random_variable.h"
#include "../distribution.h"
#include <cmath>

namespace mqf {
namespace Distributions {

	struct Rayleigh : Density<> {
		double sigma;
		
		explicit Rayleigh( double sigma = 1.0 ) : sigma(sigma) {}

		double mean() const {
			return sigma * std::sqrt(0.5*Pi);
		}

		double median() const {
			return sigma * std::sqrt(std::log(4.0));
		}

		double mode() const {
			return sigma;
		}

		double variance() const {
			return 0.5*(4.0-Pi)*sigma*sigma;
		}

		double operator()( Value x ) const {
			double y = x / sigma;
			return y * std::exp(-0.5*y*y) / sigma;
		}

		double cumulative( Value x ) const {
			double y = x / sigma;
			return 1.0 - std::exp(-0.5*y*y);
		}

	};

}
}

#endif
