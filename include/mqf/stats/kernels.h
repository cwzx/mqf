#ifndef INCLUDED_MQF_STATS_KERNELS
#define INCLUDED_MQF_STATS_KERNELS
#include <cmath>
#include "../constants.h"

namespace mqf {
namespace Kernels {

	struct Uniform {
		double operator()( double x ) const {
			return ( std::fabs(x) <= 1.0 ) ? 0.5 : 0.0;
		}

		static constexpr double squareIntegral() {
			return 0.5;
		}
	};

	struct Triangular {
		double operator()( double x ) const {
			auto mod = std::fabs(x);
			return ( mod <= 1.0 ) ? (1.0 - mod) : 0.0;
		}

		static constexpr double squareIntegral() {
			return 2.0/3.0;
		}
	};

	struct Epanechnikov {
		double operator()( double x ) const {
			if( std::fabs(x) > 1.0 )
				return 0.0;
			return 0.75 * (1.0 - x*x);
		}

		static constexpr double squareIntegral() {
			return 3.0/5.0;
		}
	};

	struct Quartic {
		double operator()( double x ) const {
			if( std::fabs(x) > 1.0 )
				return 0.0;
			auto y = 1 - x*x;
			return 0.9375 * y*y;
		}

		static constexpr double squareIntegral() {
			return 5.0/7.0;
		}
	};

	struct Triweight {
		double operator()( double x ) const {
			if( std::fabs(x) > 1.0 )
				return 0.0;
			auto y = 1 - x*x;
			return 1.09375 * y*y*y;
		}

		static constexpr double squareIntegral() {
			return 350.0/429.0;
		}
	};

	struct Tricube {
		double operator()( double x ) const {
			auto mod = std::fabs(x);
			if( mod > 1.0 )
				return 0.0;
			auto y = 1 - mod*mod*mod;
			return (70.0/81.0) * y*y*y;
		}

		static constexpr double squareIntegral() {
			return 175.0/247.0;
		}
	};
	
	struct Gaussian {
		double operator()( double x ) const {
			return std::exp(-0.5 * x*x) / std::sqrt( 2.0 * Pi );
		}

		static constexpr double squareIntegral() {
			return 1.0 / Sqrt2Pi;
		}
	};

	struct Cosine {
		double operator()( double x ) const {
			if( std::fabs(x) > 1.0 )
				return 0.0;
			return (0.25 * Pi) * std::cos( (0.5 * Pi) * x );
		}

		static constexpr double squareIntegral() {
			return (Pi*Pi) / 16.0;
		}
	};

	struct Logistic {
		double operator()( double x ) const {
			return 1.0 / ( 2.0 + std::exp(x) + std::exp(-x) );
		}

		static constexpr double squareIntegral() {
			return 1.0/6.0;
		}
	};
	
	struct Silverman {
		double operator()( double x ) const {
			auto mod = (1.0/Sqrt2) * std::fabs(x);
			return 0.5 * std::exp(-mod) * std::sin( mod + 0.25 * Pi );
		}

		static constexpr double squareIntegral() {
			return (3.0 * Sqrt2) / 16.0;
		}
	};

}
}

#endif
