#ifndef INCLUDED_MQF_COPULAS_COMONOTONICITY
#define INCLUDED_MQF_COPULAS_COMONOTONICITY
#include "../functions.h"

namespace mqf {
namespace Copulas {
	
	struct Comonotonicity {

		template<typename... Ts>
		double operator()( Ts&&... xs ) const {
			return min(std::forward<Ts>(xs)...);
		}

		template<typename... Ts>
		double density( Ts&&... ) const {
			return 0.0;
		}

	};

}
}

#endif
