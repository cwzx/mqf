#ifndef INCLUDED_MQF_DRAWDOWN
#define INCLUDED_MQF_DRAWDOWN
#include <iterator>
#include "../trading/return.h"

namespace mqf {

	struct DrawDown {
		double high, low;

		explicit DrawDown( double high = 0, double low = 0 ) : high(high), low(low) {}
		
		double loss() const {
			return high - low;
		}

		double logLoss() const {
			return -logReturn();
		}

		double lossFraction() const {
			return -Return();
		}
		
		double Return() const {
			return mqf::Return( high, low );
		}

		double logReturn() const {
			return mqf::logReturn( high, low );
		}

	};

	template<typename It>
	DrawDown drawDown( It p1, It p2 ) {
		DrawDown dd;
		if( p1 == p2 ) return dd;
		dd.low = *std::prev(p2);
		dd.high = dd.low;
		for(;p1!=p2;++p1) {
			auto x = *p1;
			if( x > dd.high )
				dd.high = x;
		}
		return dd;
	}

	template<typename It>
	DrawDown maxDrawDown( It p1, It p2 ) {
		if( p1 == p2 ) return DrawDown();
		auto peak = *p1;
		DrawDown max_dd(peak,peak);
		for(++p1;p1!=p2;++p1) {
			auto x = *p1;
			if( x > peak )
				peak = x;
			DrawDown dd( peak, x );
			if( dd.lossFraction() > max_dd.lossFraction() )
				max_dd = dd;
		}
		return max_dd;
	}

}

#endif
