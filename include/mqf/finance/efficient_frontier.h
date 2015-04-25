#ifndef INCLUDED_MQF_EFFICIENT_FRONTIER
#define INCLUDED_MQF_EFFICIENT_FRONTIER
#include <Eigen/Core>
#include <Eigen/LU>

namespace mqf {

	struct EfficientFrontier {
		using Vec = Eigen::VectorXd;
		using Mat = Eigen::MatrixXd;

		Vec assetReturns;
		Mat assetCovarianceInverse;

		double a, b, c, d;

		void set( const Vec& returns, const Mat& covariance ) {
			assetReturns = returns;
			assetCovarianceInverse = Eigen::FullPivLU<Mat>(covariance).inverse();
			double a = computeA();
			double b = computeB();
			double c = computeC();
			double d = computeD();
		}

		void setReturns( const Vec& ret ) {
			assetReturns = ret;
			double a = computeA();
			double b = computeB();
			double d = computeD();
		}

		void setCovariance( const Mat& covariance ) {
			assetCovarianceInverse = Eigen::FullPivLU<Mat>(covariance).inverse();
			double a = computeA();
			double b = computeB();
			double c = computeC();
			double d = computeD();
		}

		double minVariance() const {
			return 1.0 / c;
		}

		double returnForMinVariance() const {
			return a / c;
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
