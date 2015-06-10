#ifndef INCLUDED_MQF_TRADING_STRAT_CW1
#define INCLUDED_MQF_TRADING_STRAT_CW1
#include "../action.h"
#include "../indicators/cw1.h"

namespace mqf {
namespace Strategies {

	struct CW1 {
		double limit;

		Indicators::CW1 indicator;

		explicit CW1( double limit = 0.1875,
					  int period = 95 ) :
			limit(limit),
			indicator(period)
		{}

		template<typename It>
		Action compute( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);

			if( count < indicator.period )
				return Action( Action::Hold );

			auto h = indicator.compute(p1,p2);

			if( h.sharpe > limit ) {
				return Action( Action::Buy );
			}
			if( h.sharpe < limit ) {
				return Action( Action::Sell );
			}
			return Action( Action::Hold );
		}

	};

}
}

#endif
