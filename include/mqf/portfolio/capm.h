#ifndef INCLUDED_MQF_PORTFOLIO_CAPM
#define INCLUDED_MQF_PORTFOLIO_CAPM
#include "../stats/descriptive.h"

namespace mqf {

	/*
	 * Capital Asset Pricing Model (CAPM)
	 *
	 * Describes the excess return of an asset as an affine function of a benchmark's excess return.
	 *
	 * r = alpha + beta b + epsilon
	 *
	 * where
	 *           r : excess return of asset
	 *           b : excess return of benchmark
	 *     epsilon : random error
	 * alpha, beta : parameters
	 *
	 * The parameters that minimize E[epsilon^2] are
	 *
	 * alpha = E[r] - beta E[b]
	 *  beta = cov(r,b) / var(b)
	 *
	 */
	struct CAPM {
		double alpha, beta;
		double E_r, E_b, var_r, var_b, riskFreeRate;

		template<typename Itr, typename Itb>
		void compute( Itr r1, Itr r2, Itb b1, Itb b2, double riskFreeRate = 0.0 ) {
			E_r = sampleMean(r1,r2);
			E_b = sampleMean(b1,b2);
			var_r = sampleVariance(r1,r2);
			var_b = sampleVariance(b1,b2);
			beta = sampleCovariance( r1,r2, b1,b2 ) / var_b;
			alpha = E_r - beta * E_b
			      + riskFreeRate * (beta - 1.0);
		}

		template<typename Sr, typename Sb>
		void compute( const Sr& r, const Sb& b, double riskFreeRate = 0.0 ) {
			compute( std::begin(r), std::end(r), std::begin(b), std::end(b), riskFreeRate );
		}

		double updateRiskFreeRate( double newRate ) {
			alpha += (newRate - riskFreeRate) * (beta - 1.0);
			riskFreeRate = newRate;
		}

		double expectedExcess() const {
			return E_r - riskFreeRate;
		}

		double expectedBenchmarkExcess() const {
			return E_b - riskFreeRate;
		}

		double expectedResidual() const {
			return alpha;
		}

		double returnVariance() const {
			return var_r;
		}

		double benchmarkVariance() const {
			return var_b;
		}

		double residualVariance() const {
			return var_r - beta*beta * var_b;
		}

		double covariance() const {
			return beta * var_b;
		}

		double activeBeta() const {
			return beta - 1.0;
		}

	};

}

#endif
