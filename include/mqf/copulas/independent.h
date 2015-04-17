#ifndef INCLUDED_MQF_COPULAS_INDEPENDENT
#define INCLUDED_MQF_COPULAS_INDEPENDENT
#include "../functions.h"

namespace mqf {
namespace Copulas {

	struct Independent {

		template<typename... Ts>
		double operator()( Ts&&... xs ) const {
			return product(xs...);
		}

		template<typename... Ts>
		double density( Ts&&... xs ) const {
			return 1.0;
		}

	};

}
}

#endif
