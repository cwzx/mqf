#ifndef INCLUDED_MQF_PORTFOLIO_EFFICIENT_FRONTIER_UNCONSTRAINED
#define INCLUDED_MQF_PORTFOLIO_EFFICIENT_FRONTIER_UNCONSTRAINED
#include "../eigen_pinv.h"

namespace mqf {
	
	/*
	 * Efficient Frontier Unconstrained
	 *
	 * The set of portfolios that have the least variance for their expected return.
	 *
	 * This places no constraints on the portfolio weights.
	 *
	 */
	struct EfficientFrontierUnconstrained {
		using Vec = Eigen::VectorXd;
		using Mat = Eigen::MatrixXd;

		Vec returns;
		Mat covarianceInv;

		double retCovInvRet;

		void set( const Vec& ret, const Mat& cov ) {
			returns = ret;
			covarianceInv = pseudoInverse(cov);
			retCovInvRet = returns.dot( covarianceInv * returns );
		}

		Vec computeWeightsForReturn( double mu ) const {
			return returns * ( mu / retCovInvRet );
		}

		Vec computeWeightsForVariance( double var ) const {
			return ( covarianceInv * returns ) * std::sqrt( var / retCovInvRet );
		}

		double computeVarianceForReturn( double mu ) const {
			return (mu * mu) / retCovInvRet;
		}

		double computeReturnForVariance( double var ) const {
			return std::sqrt( var * retCovInvRet );
		}

		double computeRiskAversionForReturn( double mu ) const {
			return retCovInvRet / mu;
		}

		double computeRiskAversionForVariance( double var ) const {
			return std::sqrt( retCovInvRet / var );
		}

		Vec computeReturnsFromWeights( const Vec& weights, double riskAversion ) const {
			return riskAversion * pseudoInverse( covarianceInv ) * weights;
		}

	};

}

#endif
