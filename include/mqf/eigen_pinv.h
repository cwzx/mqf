#ifndef INCLUDED_EIGEN_PINV
#define INCLUDED_EIGEN_PINV
#include <Eigen/Core>
#include <Eigen/SVD>
#include <algorithm>

using namespace Eigen;

namespace mqf {

	template<typename T,int Rows,int Cols>
	Matrix<T,Cols,Rows> pseudoInverse( const Matrix<T,Rows,Cols>& A, typename NumTraits<T>::Real epsilon = NumTraits<T>::epsilon() ) {
		JacobiSVD<Matrix<T,Rows,Cols>> svd( A, ComputeThinU | ComputeThinV );

		NumTraits<T>::Real tolerance = epsilon
			* std::max( A.cols(), A.rows() )
			* svd.singularValues().array().abs().maxCoeff();

		JacobiSVD<Matrix<T,Rows,Cols>>::SingularValuesType svInv =
			(svd.singularValues().array().abs() > tolerance).select( svd.singularValues().array().inverse(), 0 );

		return svd.matrixV() * svInv.asDiagonal() * svd.matrixU().adjoint();
	}
}

#endif
