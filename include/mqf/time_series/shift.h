#ifndef INCLUDED_MQF_TIME_SERIES_SHIFT
#define INCLUDED_MQF_TIME_SERIES_SHIFT
#include <utility>
#include <iterator>

namespace mqf {

	template<typename T,typename F>
	struct ShiftedFunction {
		T shift;
		F f;
		
		ShiftedFunction( const T& shift, const F& f ) : shift( shift ), f( f ) {}
		
		template<typename A>
		auto operator()( A&& x ) const {
			return f( std::forward<A>(x) + shift );		
		}

	};

	template<typename T,typename F>
	ShiftedFunction<T,F> MakeShifted( T&& shift, F&& f ) {
		return ShiftedFunction<T,F>( std::forward<T>(shift), std::forward<F>(f) );
	}

	template<typename T>
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
		ShiftedFunction<T,F> operator()( F&& f ) const {
			return ShiftedFunction<T,F>( shift, std::forward<F>(f) );
		}

		ShiftOperator<T> inverse() const {
			return ShiftOperator<T>( -shift );
		}

		static ShiftOperator<T> identity() {
			return ShiftOperator<T>();
		}

	};

	template<typename T>
	ShiftOperator<T> MakeShifted( T&& shift ) {
		return ShiftOperator<T>( std::forward<T>(shift) );
	}

}

#endif
