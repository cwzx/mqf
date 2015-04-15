#ifndef INCLUDED_MQF_DRAWDOWN
#define INCLUDED_MQF_DRAWDOWN
#include "../chrono.h"
#include <iterator>
#include "../stats/descriptive.h"

namespace mqf {

	template<typename Seq>
	double DrawDown( const Seq& seq, size_t N ) {
		double max_dx = 0.0;
		for(size_t i=0;i<N;++i) {
			double dx = seq[i] - x[N];
			if( dx > max_dx ) {
				max_dx = dx;
			}
		}
		return max_dx;
	}

	template<typename Seq>
	double MaxDrawDown( const Seq& seq, size_t N ) {
		double max_dd = 0.0;
		for(size_t i=1;i<N;++i) {
			double dd = DrawDown( seq, i );
			if( dd > max_dd ) {
				max_dd = dd;
			}
		}
		return max_dd;
	}

}

#endif
