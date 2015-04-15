#ifndef INCLUDED_MQF_PROCESSES_BESSEL
#define INCLUDED_MQF_PROCESSES_BESSEL
#include "../process.h"
#include <cstdint>

namespace mqf {
namespace Processes {
	
	struct Bessel : Process<> {
		uint32_t n;

		explicit Bessel( uint32_t n = 2 ) : n(n) {}

		Value drift( Value X ) const {
			return (0.5 * ( n - 1 )) / X;
		}

		Value driftD( Value X ) const {
			return (-0.5 * ( n - 1 )) / ( X * X );
		}

		Value diffusion( Value ) const {
			return 1.0;
		}

		Value diffusionD( Value ) const {
			return 0.0;
		}
	};
	
}
}

#endif
