#ifndef INCLUDED_MQF_MULTIINDEX
#define INCLUDED_MQF_MULTIINDEX
#include <cstdint>
#include <array>
#include <initializer_list>

namespace mqf {

	template<typename T,size_t N>
	struct MultiIndex {
		std::array<T,N> index;

		MultiIndex( std::initializer_list<T> L ) : index(L) {}

		MultiIndex& operator=( std::initializer_list<T> L ) {
			index = L;
			return *this;
		}

		MultiIndex<T,N>& setZero() {
			for(size_t i=0;i<N;++i)
				index[i] = 0;
			return *this;
		}
		
		T& operator[]( size_t i ) {
			return index[i];
		}
		
		const T& operator[]( size_t i ) const {
			return index[i];
		}
		
		MultiIndex<T,N>& operator+=( const MultiIndex<T,N>& rhs ) {
			for(size_t i=0;i<N;++i)
				index[i] += rhs.index[i];
			return *this;
		}

		MultiIndex<T,N> operator+( const MultiIndex<T,N>& rhs ) const {
			MultiIndex<T,N> R = *this;
			R += rhs;
			return R;
		}
		
		MultiIndex<T,N>& operator-=( const MultiIndex<T,N>& rhsM ) {
			for(size_t i=0;i<N;i++)
				index[i] -= rhs.index[i];
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
		T r = M[0];
		for(size_t i=1;i<N;++i)
			r += M[i];
		return r;
	}
		
	template<typename T,size_t N>
	T product( const MultiIndex<T,N>& M ) {
		T r = M[0];
		for(size_t i=1;i<N;++i)
			r *= M[i];
		return r;
	}

	using Index1 = MultiIndex<uint32_t,1>;
	using Index2 = MultiIndex<uint32_t,2>;
	using Index3 = MultiIndex<uint32_t,3>;

}

#endif
