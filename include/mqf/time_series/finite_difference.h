#ifndef INCLUDED_MQF_FINITE_DIFFERENCE
#define INCLUDED_MQF_FINITE_DIFFERENCE
#include <utility>
#include <iterator>

namespace mqf {

	template<typename T,typename F>
	struct FwdDiffFunc {
		T step;
		F f;

		FwdDiffFunc( const T& step, const F& f ) : step( step ), f( f ) {}
		
		template<typename A>
		auto operator()( A&& x ) const {
			return f( std::forward<A>(x) + step ) - f(std::forward<A>(x));		
		}
	};
	
	template<typename T,typename F>
	struct BwdDiffFunc {
		T step;
		F f;

		BwdDiffFunc( const T& step, const F& f ) : step( step ), f( f ) {}
		
		template<typename A>
		auto operator()( A&& x ) const {
			return f(std::forward<A>(x)) - f( std::forward<A>(x) - step );		
		}
	};

	template<typename T,typename F>
	struct CtlDiffFunc {
		T step;
		F f;

		CtlDiffFunc( const T& step, const F& f ) : step( step ), f( f ) {}
		
		template<typename A>
		auto operator()( A&& x ) const {
			auto halfStep = 0.5 * h;
			return f(std::forward<A>(x) + halfStep ) - f( std::forward<A>(x) - halfStep );		
		}
	};


	template<typename T,typename F>
	FwdDiffFunc<T,F> MakeFwdDiff( T&& step, F&& f ) {
		return FwdDiffFunc<T,F>( std::forward<T>(step), std::forward<F>(f) );
	}

	template<typename T,typename F>
	BwdDiffFunc<T,F> MakeBwdDiff( T&& step, F&& f ) {
		return BwdDiffFunc<T,F>( std::forward<T>(step), std::forward<F>(f) );
	}

	template<typename T,typename F>
	CtlDiffFunc<T,F> MakeCtlDiff( T&& step, F&& f ) {
		return CtlDiffFunc<T,F>( std::forward<T>(step), std::forward<F>(f) );
	}

	template<typename T>
	struct FwdDiffOperator {
		T step;

		FwdDiffOperator() = default;

		explicit FwdDiffOperator( const T& shift ) : step( step ) {}

		template<typename F>
		FwdDiffFunc<T,F> operator()( F&& f ) const {
			return FwdDiffFunc<T,F>( step, std::forward<F>(f) );
		}

	};

	template<typename T>
	struct BwdDiffOperator {
		T step;

		BwdDiffOperator() = default;

		explicit BwdDiffOperator( const T& shift ) : step( step ) {}

		template<typename F>
		BwdDiffFunc<T,F> operator()( F&& f ) const {
			return BwdDiffFunc<T,F>( step, std::forward<F>(f) );
		}

	};

	template<typename T>
	struct CtlDiffOperator {
		T step;

		CtlDiffOperator() = default;

		explicit CtlDiffOperator( const T& shift ) : step( step ) {}

		template<typename F>
		CtlDiffFunc<T,F> operator()( F&& f ) const {
			return CtlDiffFunc<T,F>( step, std::forward<F>(f) );
		}

	};

	template<typename T>
	FwdDiffOperator<T> MakeFwdDiff( T&& step ) {
		return FwdDiffOperator<T>( std::forward<T>(step) );
	}

	template<typename T>
	BwdDiffOperator<T> MakeBwdDiff( T&& step ) {
		return BwdDiffOperator<T>( std::forward<T>(step) );
	}

	template<typename T>
	CtlDiffOperator<T> MakeCtlDiff( T&& step ) {
		return CtlDiffOperator<T>( std::forward<T>(step) );
	}

}

#endif
