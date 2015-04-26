#ifndef INCLUDED_MQF_POLYNOMIAL
#define INCLUDED_MQF_POLYNOMIAL
#include <Eigen/Core>

namespace mqf {

	template<typename T,int N>
	struct Polynomial {
		using Vec = Eigen::Matrix<T,N+1,1>;
		Vec coeffs;

		Polynomial() = default;

		explicit Polynomial( const Vec& rhs ) : coeffs(rhs) {}

		template<typename X>
		X operator()( const X& x ) const {
			X xn(1);
			X result = coeffs[0] * xn;
			for(int i=1;i<=N;++i) {
				xn *= x;
				result += coeffs[i] * xn;
			}
			return result;
		}

		Polynomial<T,N-1> derivative() const {
			Polynomial<T,N-1> P;
			for(int i=0;i<N;++i) {
				P.coeffs[i] = coeffs[i+1] * T(i+1);
			}
			return P;
		}

		Polynomial<T,N> operator+( const Polynomial<T,N>& rhs ) const {
			return Polynomial<T,N>( coeffs + rhs.coeffs );
		}

		Polynomial<T,N> operator-( const Polynomial<T,N>& rhs ) const {
			return Polynomial<T,N>( coeffs - rhs.coeffs );
		}

		Polynomial<T,N> operator+( T rhs ) const {
			return Polynomial<T,N>( coeffs[0] + rhs );
		}

		Polynomial<T,N> operator-( T rhs ) const {
			return Polynomial<T,N>( coeffs[0] - rhs );
		}

		Polynomial<T,N> operator*( T rhs ) const {
			return Polynomial<T,N>( coeffs * rhs );
		}

		Polynomial<T,N> operator/( T rhs ) const {
			return Polynomial<T,N>( coeffs / rhs );
		}

		template<int M>
		Polynomial<T,N+M> operator*( const Polynomial<T,M>& rhs ) const {
			Polynomial<T,N+M> P;
			P.coeffs.setZero();
			for(int i=0;i<=N;++i) {
				for(int j=0;j<=M;++j) {
					P.coeffs[i+j] = coeffs[i] * rhs.coeffs[j];
				}
			}
			return P;
		}

		Polynomial<T,N+1> operator*( const Polynomial<T,1>& rhs ) const {
			Polynomial<T,N+1> P;
			P.coeffs.setZero();
			for(int i=0;i<=N;++i) {
				for(int j=0;j<=1;++j) {
					P.coeffs[i+j] += coeffs[i] * rhs.coeffs[j];
				}
			}
			return P;
		}

		Polynomial<T,N>& operator+=( const Polynomial<T,N>& rhs ) {
			coeffs += rhs.coeffs;
			return *this;
		}

		Polynomial<T,N>& operator-=( const Polynomial<T,N>& rhs ) {
			coeffs -= rhs.coeffs;
			return *this;
		}

		Polynomial<T,N>& operator+=( T rhs ) {
			coeffs[0] += rhs;
			return *this;
		}

		Polynomial<T,N>& operator-=( T rhs ) {
			coeffs[0] -= rhs;
			return *this;
		}

		Polynomial<T,N>& operator*=( T rhs ) {
			coeffs *= rhs;
			return *this;
		}

		Polynomial<T,N>& operator/=( T rhs ) {
			coeffs /= rhs;
			return *this;
		}

	};

}

#endif
