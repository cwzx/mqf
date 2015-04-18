#ifndef INCLUDED_MQF_PDE
#define INCLUDED_MQF_PDE
#include <Eigen/Core>
#include "grid.h"

using namespace Eigen;

namespace mqf {
	/*
	 * Heat equation
	 *
	 * \partial_t f = \nabla^2 f
	 *
	 */
	struct HeatEquation {
		template<typename F>
		auto operator()( F&& f ) const {
			return laplacian( std::forward<F>(f) );
		}
	};

	struct DirichletTag {};
	struct NeumannTag {};

	template<typename F,int M,int N>
	struct Partial;
	
	template<typename F,int M,int N>
	Partial<F,M,N> partial( F&& f ) {
		return Partial<F,M,N>( std::forward<F>(f) );
	}

	template<typename F>
	struct Laplacian;

	template<typename T>
	struct Laplacian<Grid<T,1>> {

		double delta;

		explicit Laplacian( double delta = 1.0 ) : delta(delta) {}

		template<typename F>
		auto operator()( F&& f ) const {
			return [&]( auto i ) {
				return ( f(i+1) - 2.0 * f(i) + f(i-1) )
				     / ( delta * delta );
			};
		}

		template<typename Derived>
		Array<typename Derived::Scalar,-1,1> operator()( const ArrayBase<Derived>& field ) const {
			auto size = field.size();
			Array<Derived::Scalar,-1,1> lap(size);
		
			lap[0] = field[1] - 2.0 * field[0] + field[size-1];
			for(uint32_t i=1;i<size-1;++i)
				lap[i] = field[i+1] - 2.0 * field[i] + field[i-1];
			lap[size-1] = field[0] - 2.0 * field[size-1] + field[size-2];

			return lap * ( 1.0 / (delta*delta) );
		}

	};

	/*
	template<typename Derived>
	Array<typename Derived::Scalar,-1,1> gradient( const ArrayBase<Derived>& field ) const {
		Array<Derived::Scalar,-1,1> grad(size);
		
		grad[0] = field[1] - field[size-1];
		for(uint32_t i=1;i<size-1;++i)
			grad[i] = field[i+1] - field[i-1];
		grad[size-1] = field[0] - field[size-2];

		return grad * ( 1.0 / delta );
	}

	template<typename T>
	struct Grid<T,2> {
		using Index = Array<uint32_t,2,1>;
		using Real = T;
		using Position = Matrix<Real,2,1>;

		Position offset, delta;
		Index size;

		Position operator()( const Index& index ) const {
			return offset + delta.cwiseProduct( index.cast<Real>().matrix() );
		}

		explicit Grid( const Index& size ) : size(size) {
			delta.fill(1.0);
			offset.setZero();
		}

		Grid( const Index& size, const Position& delta ) : size(size), delta(delta) {
			offset.setZero();
		}

		Grid( const Index& size, const Position& delta, const Position& offset ) : size(size), delta(delta), offset(offset) {}

		template<typename Derived>
		Array<Matrix<typename Derived::Scalar,2,1>,-1,-1> gradient( const ArrayBase<Derived>& field ) const {
			uint32_t nX = size[0];
			uint32_t nY = size[1];
			Array<Derived::Scalar,-1,-1> GX(nX,nY), GY(nX,nY);
		
			GX.row(0) = field.row(1) - field.row(nX-1);
			for(uint32_t i=1;i<nX-1;++i)
				GX.row(i) = field.row(i+1) - field.row(i-1);
			GX.row(nX-1) = field.row(0) - field.row(nX-2);

			GY.col(0) = field.col(1) - field.col(nY-1);
			for(uint32_t j=1;j<nY-1;++j)
				GY.col(j) = field.col(j+1) - field.col(j-1);
			GY.col(nY-1) = field.col(0) - field.col(nY-2);

			Array<T,2,1> deltainv = delta.inverse();

			GX *= deltainv[0];
			GY *= deltainv[1];

			Array<Matrix<Derived::Scalar,2,1>,-1,-1> grad(nX,nY);
			for(uint32_t j=0;j<nY;++j) {
				for(uint32_t i=0;i<nX;++i) {
					grad(i,j)[0] = GX(i,j);
					grad(i,j)[1] = GY(i,j);
				}
			}
			return grad;
		}

		template<typename Derived>
		Array<typename Derived::Scalar,-1,-1> laplacian( const ArrayBase<Derived>& field ) const {
			uint32_t nX = size[0];
			uint32_t nY = size[1];
			Array<Derived::Scalar,-1,-1> LX(nX,nY), LY(nX,nY);
		
			LX.row(0) = field.row(1) - 2.0 * field.row(0) + field.row(nX-1);
			for(uint32_t i=1;i<nX-1;++i)
				LX.row(i) = field.row(i+1) - 2.0 * field.row(i) + field.row(i-1);
			LX.row(nX-1) = field.row(0) - 2.0 * field.row(nX-1) + field.row(nX-2);

			LY.col(0) = field.col(1) - 2.0 * field.col(0) + field.col(nY-1);
			for(uint32_t j=1;j<nY-1;++j)
				LY.col(j) = field.col(j+1) - 2.0 * field.col(j) + field.col(j-1);
			LY.col(nY-1) = field.col(0) - 2.0 * field.col(nY-1) + field.col(nY-2);

			Array<T,2,1> delta2inv = delta.inverse().square();

			return LX * delta2inv[0] + LY * delta2inv[1];
		}

		template<typename Derived>
		Array<typename Derived::Scalar,-1,-1> partial( uint8_t d, const ArrayBase<Derived>& field ) const {
			uint32_t nX = size[0];
			uint32_t nY = size[1];
			Array<Derived::Scalar,-1,-1> P(nX,nY);

			if( d == 0 ) {
				P.row(0) = field.row(1) - field.row(nX-1);
				for(uint32_t i=1;i<nX-1;++i)
					P.row(i) = field.row(i+1) - field.row(i-1);
				P.row(nX-1) = field.row(0) - field.row(nX-2);
			} else if( d == 1 ) {
				P.col(0) = field.col(1) - field.col(nY-1);
				for(uint32_t j=1;j<nY-1;++j)
					P.col(j) = field.col(j+1) - field.col(j-1);
				P.col(nY-1) = field.col(0) - field.col(nY-2);
			}
			return P * ( 1.0 / delta[d] );
		}

	};
	*/
}

#endif
