#ifndef INCLUDED_MQF_DRAWDOWN
#define INCLUDED_MQF_DRAWDOWN
#include <iterator>

namespace mqf {

	template<typename It>
	double drawDown( It p1, It p2 ) {
		if( p1 == p2 ) return 0.0;
		auto max_dx = 0.0;
		auto last = *std::prev(p2);
		for(;p1!=p2;++p1) {
			auto dx = *p1 - last;
			if( dx > max_dx ) {
				max_dx = dx;
			}
		}
		return max_dx;
	}

	template<typename It>
	double maxDrawDown( It p1, It p2 ) {
		auto max_dd = 0.0;
		auto e = std::next(p2);
		for(auto q = std::next(p1);q!=e;++q) {
			auto dd = drawDown( p1, q );
			if( dd > max_dd ) {
				max_dd = dd;
			}
		}
		return max_dd;
	}

}

#endif
