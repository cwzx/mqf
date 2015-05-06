#ifndef INCLUDED_MQF_PDE_GRID
#define INCLUDED_MQF_PDE_GRID
#include "multi_index.h"
#include <cstddef>
#include <iterator>

namespace mqf {

	template<typename T,size_t N>
	struct GridIterator {
		using this_type         = GridIterator<T,N>;
		using value_type        = MultiIndex<T,N>;
		using reference         = value_type&;
		using pointer           = value_type*;
		using difference_type   = std::ptrdiff_t;
		using iterator_category = std::bidirectional_iterator_tag;

		MultiIndex<T,N> index, size;

		GridIterator( const MultiIndex<T,N>& index,
		              const MultiIndex<T,N>& size ) :
			index(index),
			size(size)
		{}
		
		reference operator*() {
			return index;
		}

		pointer operator->() {
			return &index;
		}

		this_type& operator++() {
			for(size_t i=0;i<N;++i) {
				++index[i];
				if( index[i] == size[i] ) {
					if( i < N-1 )
						index[i] = 0;
				} else break;
			}
			return *this;
		}

		this_type operator++(int) {
			auto r = *this;
			++(*this);
			return r;
		}

		this_type& operator--() {
			for(size_t i=0;i<N;++i) {
				if( index[i] == 0 ) {
					index[i] = size[i] - 1;
				} else {
					--index[i];
					break;
				}
			}
			return *this;
		}

		this_type operator--(int) {
			auto r = *this;
			--(*this);
			return r;
		}

		bool operator==( const this_type& rhs ) const {
			return (index == rhs.index).all() && (size == rhs.size).all();
		}

		bool operator!=( const this_type& rhs ) const {
			return !(*this == rhs);
		}

	};
	
	template<typename T,int N>
	struct Grid {
		using value_type = T;
		using iterator = GridIterator<value_type,N>;
		using const_iterator = iterator;

		MultiIndex<value_type,N> size;

		iterator begin() const {
			return iterator( MultiIndex<value_type,N>::Zero(), size );
		}

		iterator end() const {
			auto e = begin();
			return ++(--e);
		}

	};

}

#endif
