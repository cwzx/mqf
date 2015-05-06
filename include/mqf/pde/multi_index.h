#ifndef INCLUDED_MQF_MULTIINDEX
#define INCLUDED_MQF_MULTIINDEX
#include <cstdint>
#include <Eigen/Core>
#include <initializer_list>

namespace mqf {

	template<typename T,size_t N>
	using MultiIndex = Eigen::Array<T,N,1>;

	/*
	template<typename T,size_t N>
	struct MultiIndex {
		Eigen::Array<T,N,1> index;

		MultiIndex( std::initializer_list<T> L ) : index(L) {}

		MultiIndex& operator=( std::initializer_list<T> L ) {
			index = L;
			return *this;
		}

		MultiIndex<T,N>& setZero() {
			index.setZero();
			return *this;
		}
		
		T& operator[]( size_t i ) {
			return index[i];
		}
		
		const T& operator[]( size_t i ) const {
			return index[i];
		}
		
		MultiIndex<T,N>& operator+=( const MultiIndex<T,N>& rhs ) {
			index += rhs;
			return *this;
		}

		MultiIndex<T,N> operator+( const MultiIndex<T,N>& rhs ) const {
			MultiIndex<T,N> R = *this;
			R += rhs.index;
			return R;
		}
		
		MultiIndex<T,N>& operator-=( const MultiIndex<T,N>& rhsM ) {
			index -= rhs.index;
			return *this;
		}
		
		MultiIndex<T,N> operator-( const MultiIndex<T,N>& rhs ) const {
			MultiIndex<T,N> R = *this;
			R -= rhs;
			return R;
		}
		
		bool operator<( const MultiIndex<T,N>& rhs ) const {
			bool r = true;
			for(size_t i=0;i<N;i++)
				if( !(index[i] < rhs.index[i]) )
					return false;
			return r;
		}
		
		bool operator>( const MultiIndex<T,N>& rhs ) const {
			bool r = true;
			for(size_t i=0;i<N;i++)
				if( !(index[i] > rhs.index[i]) )
					return false;
			return r;
		}
		
		bool operator<=( const MultiIndex<T,N>& rhs ) const {
			bool r = true;
			for(size_t i=0;i<N;i++)
				if( !(index[i] <= rhs.index[i]) )
					return false;
			return r;
		}
		
		bool operator>=( const MultiIndex<T,N>& rhs ) const {
			bool r = true;
			for(size_t i=0;i<N;i++)
				if( !(index[i] >= rhs.index[i]) )
					return false;
			return r;
		}

	};

	template<typename T,size_t N>
	T sum( const MultiIndex<T,N>& M ) {
		return M.index.sum();
	}
		
	template<typename T,size_t N>
	T product( const MultiIndex<T,N>& M ) {
		return M.index.prod();
	}

	using Index1 = MultiIndex<uint32_t,1>;
	using Index2 = MultiIndex<uint32_t,2>;
	using Index3 = MultiIndex<uint32_t,3>;
	*/
}

#endif
