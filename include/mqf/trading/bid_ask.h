#ifndef INCLUDED_MQF_TRADING_BID_ASK
#define INCLUDED_MQF_TRADING_BID_ASK

namespace mqf{

	struct BidAsk {
		double bid, ask;

		explicit BidAsk( double bid = 0.0, double ask = 0.0 ) : bid(bid), ask(ask) {}

		double spread() const {
			return ask - bid;
		}

		double midpoint() const {
			return 0.5 * (bid + ask);
		}

	};

}

#endif
