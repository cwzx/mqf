#ifndef INCLUDED_MQF_DISTRIBUTION
#define INCLUDED_MQF_DISTRIBUTION
#include <random>
#include "constants.h"

namespace mqf {

	template<typename Val = double>
	struct Distribution {
		using Value = Val;
	};

	template<typename Value = double>
	struct Density : Distribution<Value> {};

	template<typename Value = int>
	struct Mass : Distribution<Value> {};

}

#endif
