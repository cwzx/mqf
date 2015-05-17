#ifndef INCLUDED_EIGEN_PINV
#define INCLUDED_EIGEN_PINV
#include <Eigen/Core>
#include <Eigen/SVD>
#include <algorithm>

namespace mqf {

	template<typename Derived>
	Eigen::Matrix<typename Derived::Scalar,
	              Derived::ColsAtCompileTime,
	              Derived::RowsAtCompileTime>
	pseudoInverse( const Eigen::MatrixBase<Derived>& A ) {
		using namespace Eigen;
		using T = Derived::Scalar;
		constexpr int Rows = Derived::RowsAtCompileTime;
		constexpr int Cols = Derived::ColsAtCompileTime;
		//JacobiSVD<Matrix<T,Rows,Cols>> svd( A, ComputeThinU | ComputeThinV );
		BDCSVD<Matrix<T,Rows,Cols>> svd( A, ComputeThinU | ComputeThinV );

		auto threshold = svd.threshold();

		decltype(svd)::SingularValuesType svInv = ( svd.singularValues().array().abs() > threshold ).select( svd.singularValues().array().inverse(), 0 );

		return svd.matrixV() * svInv.asDiagonal() * svd.matrixU().adjoint();
	}

}

#endif
