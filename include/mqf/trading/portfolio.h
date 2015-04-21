#ifndef INCLUDED_MQF_PORTFOLIO
#define INCLUDED_MQF_PORTFOLIO
#include "../kv_vector.h"
#include "../gregorian.h"

namespace mqf {

	using SecurityID = uint32_t;

	struct Portfolio {
		kv_vector<SecurityID,uint32_t> positions;
		double cash;

		double initial_value;
		Gregorian::Date initial_date;
	};

}

#endif
