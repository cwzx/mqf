#ifndef INCLUDED_MQF_PDE_GRID
#define INCLUDED_MQF_PDE_GRID
#include "multi_index.h"
#include <cstddef>
#include <iterator>

namespace mqf {

	/*
	 * A rectangular grid of points.
	 *
	 * T : point type
	 * N : dimension
	 *
	 */
	template<typename T,int N>
	struct Grid;

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

	};

	template<typename T>
	struct Grid<T,1> {
		using Index = uint32_t;
		using Point = T;
		using iterator = Index;

		Point offset, delta;
		Index size;

		explicit Grid( Index size = 1024,
		               Point delta = 1.0,
		               Point offset = 0.0 ) :
			size(size),
			delta(delta),
			offset(offset)
		{}

		Point operator()( Index index ) const {
			return offset + delta * index;
		}

		iterator begin() const {
			return 0;
		}

		iterator end() const {
			return size;
		}

	};
	

}

#endif
