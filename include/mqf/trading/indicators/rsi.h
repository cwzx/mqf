#ifndef INCLUDED_MQF_TRADING_INDICATORS_RSI
#define INCLUDED_MQF_TRADING_INDICATORS_RSI
#include "../../time_series/ema.h"
#include "../return.h"
#include <algorithm>
#include <iterator>

namespace mqf {
namespace Indicators {

	struct RS {
		using Value = double;

		EMA ema;

		explicit RS( int period = 14 ) : ema(1.0/period) {}

		template<typename It>
		std::vector<double> compute_up( It p1, It p2 ) const {
			std::vector<double> up;
			up.reserve( std::distance(p1,p2) );
			std::transform( p1, p2, std::back_inserter(up), [](double x){ return std::fmax(0.0,x); } );
			return up;
		}

		template<typename It>
		std::vector<double> compute_down( It p1, It p2 ) const {
			std::vector<double> down;
			down.reserve( std::distance(p1,p2) );
			std::transform( p1, p2, std::back_inserter(down), [](double x){ return std::fmax(0.0,-x); } );
			return down;
		}

		template<typename It>
		Value compute( It p1, It p2 ) const {
			auto up = compute_up(p1,p2);
			auto down = compute_down(p1,p2);
			auto ema_up = ema.back( up.begin(), up.end() );
			auto ema_down = ema.back( down.begin(), down.end() );
			return ema_up / ema_down;
		}

	};

	struct RSI {
		using Value = double;

		RS rs;

		explicit RSI( int period = 14 ) : rs(period) {}

		template<typename It>
		Value compute( It p1, It p2 ) const {
			auto rs_factor = rs.compute(p1,p2);
			return 100.0 * rs_factor / (1 + rs_factor);
		}

	};

}
}

#endif
