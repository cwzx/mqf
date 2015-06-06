#ifndef INCLUDED_MQF_PORTFOLIO
#define INCLUDED_MQF_PORTFOLIO
#include "../gregorian.h"
#include <vector>
#include <Eigen/Core>

namespace mqf {

	struct Portfolio {
		using Vec = Eigen::VectorXd;

		Vec weights;
		std::vector<uint32_t> ids;

	};

}

#endif
