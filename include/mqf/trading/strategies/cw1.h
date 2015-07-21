#ifndef INCLUDED_MQF_TRADING_STRAT_CW1
#define INCLUDED_MQF_TRADING_STRAT_CW1
#include "../action.h"
#include "../indicators/cw1.h"

namespace mqf {
namespace Strategies {

	struct CW1 {
		double longLimit;

		Indicators::CW1 indicator;

		explicit CW1( double longLimit = 0.01,
					  int period = 100 ) :
			longLimit(longLimit),
			indicator(period)
		{}

		template<typename It>
		Action compute( It p1, It p2 ) const {
			auto count = std::distance(p1,p2);

			if( count < indicator.period )
				return Action( Action::Out );

			auto h = indicator.compute(p1,p2);

			if( h.sharpe > longLimit ) {
				return Action( Action::Long );
			}
			return Action( Action::Out );
		}

	};

}
}

#endif
