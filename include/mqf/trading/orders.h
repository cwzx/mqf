#ifndef INCLUDED_MQF_TRADING_ORDERS
#define INCLUDED_MQF_TRADING_ORDERS
#include <cstdint>

namespace mqf {

	template<typename BuySell,typename Type,typename Lifetime>
	struct Order {
		uint64_t volume;
	};

	struct Buy {};
	struct Sell {};
	
	struct Market {};             // Fill immediately at the best available price
	struct MarketOnClose {};      // Fill at the close at the best available price
	
	struct Limit {};              // Fill at the specified price
	struct AtOrBetter {};         // Fill at the specified price or better

	struct StopLoss {};           // Becomes a market order when the price drops below a given value
	
	// Lifetime

	struct GoodTilCancelled {};   // Keep open until filled
	struct FillOrKill {};         // Fill immediately and completely or cancel
	struct ImmediateOrCancel {};  // Fill immediately (possibly partially) and cancel the remaining volume
	struct AllOrNone {};          // Fill only when there is enough supply to satisfy the full volume

}

#endif
