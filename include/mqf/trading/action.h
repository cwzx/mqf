#ifndef INCLUDED_MQF_TRADING_ACTION
#define INCLUDED_MQF_TRADING_ACTION

namespace mqf {

	struct Action {
		enum actionType { Out, Long, Short } type;
		double amount;

		explicit Action( actionType type = Out, double amount = 1.0 ) : type(type), amount(amount) {}
	};

}

#endif
