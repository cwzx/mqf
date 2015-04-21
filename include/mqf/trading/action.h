#ifndef INCLUDED_MQF_TRADING_ACTION
#define INCLUDED_MQF_TRADING_ACTION
#include <mqf/stats/descriptive.h>

namespace mqf {

	struct Action {
		enum actionType { Hold, Buy, Sell } type;
		double amount;

		explicit Action( actionType type = Hold, double amount = 0.0 ) : type(type), amount(amount) {}
	};

}

#endif
