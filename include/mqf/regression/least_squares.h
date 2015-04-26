#ifndef INCLUDED_MQF_LEAST_SQUARES
#define INCLUDED_MQF_LEAST_SQUARES
#include <Eigen/Core>
#include <Eigen/LU>
#include "../eigen_pinv.h"
#include <cstdint>

namespace mqf {

	using namespace Eigen;

	/*
	 * Ordinary Least Squares (OLS)
	 *
	 * Y : observed outputs
	 * X : design matrix, function of inputs
	 *
	 */
	template<typename T,int XRows,int XCols,int YCols>
	Matrix<T,XCols,YCols> OLS( const Matrix<T,XRows,YCols>& Y,
	                           const Matrix<T,XRows,XCols>& X ) {
		auto A = X.adjoint() * X;
		FullPivLU<MatrixXd> lu(A);
		return lu.solve( X.adjoint() * Y );
	}

	/*
	 * OLS variance estimator
	 *
	 * residuals : Y - X * beta
	 *       dof : statistical degrees of freedom
	 *
	 */
	template<typename T,int YRows,int YCols>
	T varianceEstimatorOLS( const Matrix<T,YRows,YCols>& residuals, uint32_t dof ) {
		T s2 = residuals.array().square().sum();
		return s2 / dof;
	}

	/*
	 * White's HCE variance estimator for OLS
	 *
	 * residuals : Y - X * beta
	 *         X : design matrix, function of inputs
	 *
	 */
	template<typename T,int XRows,int XCols,int YCols>
	Matrix<T,XCols,YCols> varianceEstimatorOLSWhite( const Matrix<T,XRows,YCols>& residuals,
	                                                 const Matrix<T,XRows,XCols>& X ) {
		auto psX = pseudoInverse( X );
		return psX * residuals.array().square().matrix().asDiagonal() * psX.adjoint();
	}

	/*
	 * Generalized Least Squares (GLS)
	 *
	 *        Y : observed outputs
	 *        X : design matrix, function of inputs
	 * OmegaInv : Inverse of var[ error | X ]
	 *
	 */
	template<typename T,int XRows,int XCols,int YCols>
	Matrix<T,XCols,YCols> GLS( const Matrix<T,XRows,YCols>& Y,
	                           const Matrix<T,XRows,XCols>& X,
	                           const Matrix<T,XRows,XRows>& OmegaInv ) {
		auto A = X.adjoint() * OmegaInv * X;
		FullPivLU<MatrixXd> lu(A);
		return lu.solve( X.adjoint() * OmegaInv * Y );
	}

	/*
	 * Feasible Generalized Least Squares (FGLS)
	 *
	 *        Y : observed outputs
	 *        X : design matrix, function of inputs
	 * OmegaInv : Inverse of var[ error | X ]
	 *
	 */
	template<typename T,int XRows,int XCols,int YCols>
	Matrix<T,XCols,YCols> FGLS( const Matrix<T,XRows,YCols>& Y,
	                            const Matrix<T,XRows,XCols>& X,
	                            const Matrix<T,XRows,XRows>& OmegaInv ) {
		auto betaOLS = OLS(Y,X);
		auto residuals = Y - X * betaOLS;
		auto Omega = covarianceEstimator( betaOLS, residuals ); // todo
		return GLS( Y, X, pseudoInverse( Omega ) );
	}


}

#endif
