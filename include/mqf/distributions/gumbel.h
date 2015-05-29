#ifndef INCLUDED_MQF_DISTRIBUTIONS_GUMBEL
#define INCLUDED_MQF_DISTRIBUTIONS_GUMBEL
#include <cmath>
#include <random>
#include "../distribution.h"
#include "../constants.h"
#include "../functions.h"

namespace mqf {
namespace Distributions {

	struct Gumbel : Density<> {
		double mu, beta;

		explicit Gumbel( double mu = 0.0, double beta = 1.0 ) : mu(mu), beta(beta) {}

		double mean() const {
			return mu + beta * EulerMascheroni;
		}

		double median() const {
			return mu - beta * std::log( std::log( 2.0 ) );
		}

		double mode() const {
			return mu;
		}

		double variance() const {
			return square(Pi * beta) / 6.0;
		}

		double skewness() const {
			return 1.1395470994046486574927930193898;
		}

		double excessKurtosis() const {
			return 2.4;
		}

		double operator()( double x ) const {
			double z = (x - mu) / beta;
			return std::exp( -(z + std::exp(-z)) ) / beta;
		}

		double cumulative( double x ) const {
			return std::exp( -std::exp(-(x - mu)/beta) );
		}

		std::extreme_value_distribution<Value> distribution() const {
			return std::extreme_value_distribution<Value>( mu, beta );
		}

	};

	struct StdGumbel : Density<> {

		double mean() const {
			return EulerMascheroni;
		}

		double median() const {
			return -std::log( std::log( 2.0 ) );
		}

		double mode() const {
			return 0.0;
		}

		double variance() const {
			return Pi*Pi/6.0;
		}

		double skewness() const {
			return 1.1395470994046486574927930193898;
		}

		double excessKurtosis() const {
			return 2.4;
		}

		double operator()( double x ) const {
			return std::exp( -(x + std::exp(-x)) );
		}

		double cumulative( double x ) const {
			return std::exp( -std::exp(-x) );
		}

		std::extreme_value_distribution<Value> distribution() const {
			return std::extreme_value_distribution<Value>( 0, 1 );
		}

		operator Gumbel() const {
			return Gumbel(0,1);
		}

	};

}

/* mle

1/N sum_i x_i exp(-(x_i - mu)/beta) = x_bar - beta
1/N sum_i exp(-(x_i - mu)/beta) = 1

*/

	template<typename>
	struct MomentEstimation;

	template<>
	struct MomentEstimation<Distributions::Gumbel> {
		using Dist = Distributions::Gumbel;
		template<typename It>
		Dist operator()( It p1, It p2 ) const {
			auto mu = sampleMean(p1,p2);
			auto var = sampleVariance(p1,p2,mu);
			auto beta = std::sqrt( 6.0 * var ) / Pi;
			return Dist( mu - beta * EulerMascheroni, beta );
		}
	};

}

#endif
