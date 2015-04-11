#ifndef INCLUDED_MQF_UTILITY
#define INCLUDED_MQF_UTILITY
#include <numeric>

namespace mqf {

	template<typename Container>
	auto accumulate( const Container& c, typename Container::value_type initial ) {
		return std::accumulate( std::begin(c), std::end(c), initial );
	}

}

#endif
