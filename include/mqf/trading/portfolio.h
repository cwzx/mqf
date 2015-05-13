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

		double cash;

		void normalize() {
			double norm = weights.cwiseAbs().sum() + cash;
			weights /= norm;
			cash /= norm;
		}

	};

}

#endif
