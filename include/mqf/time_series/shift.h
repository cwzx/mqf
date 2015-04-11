#ifndef INCLUDED_MQF_TIME_SERIES_SHIFT
#define INCLUDED_MQF_TIME_SERIES_SHIFT
#include <utility>
#include <iterator>
#include "../data/range.h"

namespace mqf {

	template<typename F,typename T>
	struct ShiftedFunction {
		F f;
		T shift;

		ShiftedFunction( const F& f, const T& shift ) :
			f( f ),
			shift( shift )
		{}
		
		template<typename A>
		auto operator()( A&& x ) const {
			return f( std::forward<A>(x) + shift );		
		}

	};

	template<typename F,typename T>
	ShiftedFunction<F,T> MakeShifted( F&& f, T&& shift ) {
		return ShiftedFunction<F,T>( std::forward<F>(f), std::forward<T>(shift) );
	}

	template<typename T = int>
	struct ShiftOperator {
		T shift;

		ShiftOperator() = default;

		explicit ShiftOperator( const T& shift ) : shift( shift ) {}

		// compose shifts
		ShiftOperator<T> operator*( const ShiftOperator<T>& rhs ) const {
			return ShiftOperator<T>( shift + rhs.shift );
		}
		
		// apply shift
		template<typename F>
		ShiftedFunction<F,T> operator()( F&& f ) const {
			return ShiftedFunction<F,T>( std::forward<F>(f), shift );
		}

		// apply shift -- alternate notation
		template<typename F>
		ShiftedFunction<F,T> operator*( F&& f ) const {
			return ShiftedFunction<F,T>( std::forward<F>(f), shift );
		}

		ShiftOperator<T> inverse() const {
			return ShiftOperator<T>( -shift );
		}

		ShiftOperator<T> identity() const {
			return ShiftOperator<T>();
		}

	};

}

#endif
