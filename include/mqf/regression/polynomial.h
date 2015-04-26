#ifndef INCLUDED_MQF_REGRESSION_POLYNOMIAL
#define INCLUDED_MQF_REGRESSION_POLYNOMIAL
#include <Eigen/Core>
#include "least_squares.h"

namespace mqf {

	/*
	 * Polynomial Regression
	 *
	 *      Y : outputs
	 *      X : inputs
	 * degree : polynomial degree
	 *
	 */
	template<typename T,int Rows>
	Matrix<T,-1,-1> PolynomialRegression( const Matrix<T,Rows,1>& Y,
	                                      const Matrix<T,Rows,1>& X,
	                                      uint32_t degree ) {
		Matrix<T,-1,-1> D( Y.rows(), degree + 1 );
		D.col(0).setOnes();
		for(uint32_t i=1;i<=degree;++i) {
			D.col(i) = D.col(i-1).cwiseProduct(X);
		}
		return OLS( Y, D );
	}


}

#endif
