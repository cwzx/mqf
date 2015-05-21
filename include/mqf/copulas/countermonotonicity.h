#ifndef INCLUDED_MQF_COPULAS_COUNTERMONOTONICITY
#define INCLUDED_MQF_COPULAS_COUNTERMONOTONICITY
#include "../functions.h"

namespace mqf {
namespace Copulas {
	
	struct Countermonotonicity {

		template<typename... Ts>
		double operator()( Ts&&... xs ) const {
			return std::fmax( sum(std::forward<Ts>(xs)...) + 1.0 - sizeof...(xs), 0.0 );
		}

	};

}
}

#endif
