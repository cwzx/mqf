#ifndef INCLUDED_MQF_UTILITY
#define INCLUDED_MQF_UTILITY
#include <numeric>

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

}

#endif
