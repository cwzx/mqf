#ifndef INCLUDED_MQF_PORTFOLIO_EFFICIENT_FRONTIER
#define INCLUDED_MQF_PORTFOLIO_EFFICIENT_FRONTIER
#include "../eigen_pinv.h"

namespace mqf {

	/*
	 * Efficient Frontier
	 *
	 * The set of portfolios that have the least variance for their expected return.
	 *
	 * This uses the fully-invested constraint \sum_i w_i = 1.
	 *
	 * The tangency portfolio is the portfolio on the efficient frontier
	 * that maximizes the Sharpe ratio w.r.t. a given risk-free rate.
	 *
	 */
	struct EfficientFrontier {
		using Vec = Eigen::VectorXd;
		using Mat = Eigen::MatrixXd;

		Vec assetReturns;
		Mat assetCovarianceInverse;

		double a, b, c, d;

		void set( const Vec& returns, const Mat& covariance ) {
			assetReturns = returns;
			assetCovarianceInverse = pseudoInverse(covariance);
			a = computeA();
			b = computeB();
			c = computeC();
			d = computeD();
		}

		void setReturns( const Vec& ret ) {
			assetReturns = ret;
			a = computeA();
			b = computeB();
			d = computeD();
		}

		void setCovariance( const Mat& covariance ) {
			assetCovarianceInverse = pseudoInverse(covariance);
			a = computeA();
			b = computeB();
			c = computeC();
			d = computeD();
		}

		double minVariance() const {
			return 1.0 / c;
		}

		double returnForMinVariance() const {
			return a / c;
		}

		double returnOfTangency( double riskFreeRate = 0.0 ) const {
			return ( riskFreeRate * a - b ) / ( riskFreeRate * c - a );
		}

		double varianceForReturn( double mu ) const {
			return ( c * mu*mu - 2.0 * a * mu + b ) / d;
		}

		Vec computePortfolioWithReturn( double mu ) const {
			auto N = assetReturns.size();
			auto g = assetCovarianceInverse * ( Vec::Ones(N) * b - assetReturns * a ) / d;
			auto h = assetCovarianceInverse * ( assetReturns * c - Vec::Ones(N) * a ) / d;
			return g + h * mu;
		}

		Vec computeMinVariancePortfolio() const {
			auto N = assetReturns.size();
			auto g = assetCovarianceInverse * ( Vec::Ones(N) * b - assetReturns * a ) / d;
			auto h = assetCovarianceInverse * ( assetReturns * c - Vec::Ones(N) * a ) / d;
			return g + h * (a / c);
		}

		Vec computeTangencyPortfolio( double riskFreeRate = 0.0 ) const {
			return computePortfolioWithReturn( returnOfTangency(riskFreeRate) );
		}

		double computeA() const {
			auto N = assetReturns.size();
			return assetReturns.dot( assetCovarianceInverse * Vec::Ones(N) );
		}

		double computeB() const {
			return assetReturns.dot( assetCovarianceInverse * assetReturns );
		}

		double computeC() const {
			auto N = assetReturns.size();
			return Vec::Ones(N).dot( assetCovarianceInverse * Vec::Ones(N) );
		}

		double computeD() const {
			return b*c - a*a;
		}

	};

}

#endif
