#ifndef INCLUDED_MQF_TRADING_TRANSACTION_COSTS
#define INCLUDED_MQF_TRADING_TRANSACTION_COSTS
#include <cstdint>
#include <limits>
#include "../utility.h"

namespace mqf{

	struct TransactionFee {

		double feePerValue = 0.0,
		        feePerUnit = 0.0,
		       minPerOrder = 0.0,
		       maxPerOrder = std::numeric_limits<double>::max();

		double compute( double unitPrice, uint64_t volume ) const {
			double fee = ( feePerUnit + feePerValue * unitPrice ) * volume;
			return clamp( fee, minPerOrder, maxPerOrder );
		}
	
	};

}

#endif
