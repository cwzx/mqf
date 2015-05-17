#ifndef INCLUDED_MQF_BLACK_LITTERMAN
#define INCLUDED_MQF_BLACK_LITTERMAN
#include "../eigen_pinv.h"

namespace mqf {

	/*
	 * The Black--Litterman Model
	 *
	 * A Bayesian approach to portfolio construction.
	 * Starts with a market portfolio and incorporates
	 * multiple views specified by the user.
	 *
	 */
	struct BlackLitterman {
		using Vec = Eigen::VectorXd;
		using Mat = Eigen::MatrixXd;
		using Diag = Eigen::DiagonalMatrix<double,-1>;

		double riskFreeRate = 0.0,
		       riskAversion = 1.0,
		       tau = 1.0 / 40;

		Vec returns;     // expected excess return of each asset
		Mat covariance;  // covariance of the asset returns

		/*
		 * Views
		 *
		 * A view is a portfolio with a prediction of its performance (excess return)
		 * and a variance that quantifies the uncertainty in this prediction.
		 *
		 * Multiple views over a set of assets are combined into vectors/matrices.
		 *
		 */
		struct Views {
			Mat weights;       // asset weights defining each view portfolio -- each row sums to either 0 (relative view) or 1 (absolute view) (size k-by-n)
			Vec returns;       // predicted excess return of each view (size k)
			Diag variances;    // variances of the predicted returns (size k)

			Views() = default;
			explicit Views( int nViews, int nAssets ) : weights(nViews,nAssets), returns(nViews), variances(nViews) {}

			void setVarWithTauSigma( double tau, const Mat& covariance ) {
				variances.diagonal() = tau * ( weights * covariance * weights.transpose() ).diagonal();
			}
		};

		void setMarketWeights( const Vec& weights ) {
			returns = computeMarketReturns( weights );
		}

		void setMarket( const Vec& weights, const Mat& cov ) {
			covariance = cov;
			returns = computeMarketReturns( weights );
		}

		Vec computeMarketReturns( const Vec& weights ) const {
			return riskAversion * covariance * weights;
		}

		Vec computePosteriorReturns( const Views& vs ) const {
			return returns + tau * covariance * vs.weights.transpose()
			     * pseudoInverse( ( tau * vs.weights * covariance * vs.weights.transpose() + vs.variances.diagonal() ).eval() )
			     * ( vs.returns - vs.weights * returns );
		}

		Mat computePosteriorVariance( const Views& vs ) const {
			auto tauSigma = tau * covariance;
			return tauSigma - tauSigma * vs.weights.transpose()
			     * pseudoInverse( ( vs.weights * tauSigma * vs.weights.transpose() + vs.variances.diagonal() ).eval() )
			     * vs.weights * tauSigma;
		}

		Vec computeWeights( const Views& vs ) const {
			auto posteriorReturns = computePosteriorReturns( vs );
			auto posteriorVariance = computePosteriorVariance( vs );
			return (1.0 / riskAversion) * posteriorReturns * pseudoInverse( ( covariance + posteriorVariance ).eval() );
		}

	};

	

}

#endif
