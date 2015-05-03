#ifndef INCLUDED_MQF_UTILITY
#define INCLUDED_MQF_UTILITY
#include <numeric>
#include <cstdint>

namespace mqf {

	template<typename Container>
	auto accumulate( const Container& c, typename Container::value_type initial ) {
		return std::accumulate( std::begin(c), std::end(c), initial );
	}
	
	template<typename T,typename F>
	void plot( const char* filename, T lower, T upper, uint32_t N, F&& f ) {
		std::ofstream out(filename);

		T delta = ( upper - lower ) / ( N - 1 );
		T x = lower;
		for(uint32_t i=0;i<N;++i) {
			out << x << "," << std::forward<F>(f)(x) << endl;
			x += delta;
		}
		
	}

	template<typename T,typename F>
	void plot2D( const char* filename, T lower1, T upper1,  T lower2, T upper2, uint32_t N1, uint32_t N2, F&& f ) {
		std::ofstream out(filename);

		T delta1 = ( upper1 - lower1 ) / ( N1 - 1 );
		T delta2 = ( upper2 - lower2 ) / ( N2 - 1 );
		
		out << ",";
		for(uint32_t j=0;j<N2;++j) {
			out << lower2 + delta2*j << ",";
		}
		out << endl;

		T x = lower1;
		for(uint32_t i=0;i<N1;++i) {
			out << x << ",";
			T y = lower2;
			for(uint32_t j=0;j<N2;++j) {
				out << std::forward<F>(f)(x,y) << ",";
				y += delta2;
			}
			out << endl;
			x += delta1;
		}
		
	}

	template<typename T>
	T clamp( T x, T lower, T upper ) {
		if( x < lower ) x = lower;
		if( x > upper ) x = upper;
		return x;
	}

}

#endif
