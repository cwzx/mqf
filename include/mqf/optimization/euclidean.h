#ifndef INCLUDED_MQF_OPTIMIZATION_EUCLIDEAN
#define INCLUDED_MQF_OPTIMIZATION_EUCLIDEAN
#include <Eigen/Core>

namespace mqf {

	template<typename Pnt,typename IP>
	struct UniformMetric {
		using Point = Pnt;
		using InnerProduct = IP;

		InnerProduct operator()( const Point& ) const {
			return InnerProduct();
		}

	};

namespace Euclidean {

	template<typename V>
	struct DotProduct {
		using Vector = V;
		using Scalar = typename V::Scalar;

		auto operator()( const Vector& x, const Vector& y ) const {
			return y.dot(x);
		}

		auto norm2( const Vector& x ) const {
			return x.squaredNorm();
		}

	};

	template<typename Mat>
	struct FrobeniusInnerProduct {
		using Vector = Mat;
		using Scalar = typename Mat::Scalar;

		auto operator()( const Vector& x, const Vector& y ) const {
			return ( y.adjoint() * x ).trace();
		}

		auto norm2( const Vector& x ) const {
			return x.squaredNorm();
		}

	};

	template<typename>
	struct InnerProduct;

	template<typename Scalar,int Rows>
	struct InnerProduct<Eigen::Matrix<Scalar,Rows,1>> : DotProduct<Eigen::Matrix<Scalar,Rows,1>> {};

	template<typename Scalar,int Rows,int Cols>
	struct InnerProduct<Eigen::Matrix<Scalar,Rows,Cols>> : FrobeniusInnerProduct<Eigen::Matrix<Scalar,Rows,Cols>> {};

	template<typename V>
	struct Geodesic {
		using Metric = UniformMetric<V,InnerProduct<V>>;
		using Vector = V;
		using T = typename V::Scalar;
		
		Vector position, velocity;

		void set( const Vector& x, const Vector& v ) {
			position = x;
			velocity = v;
		}

		auto operator()( T t ) const {
			return position + velocity * t;
		}

		Vector parallelTranslate( const Vector& v, T ) const {
			return v;
		}

	};

}
}

#endif

