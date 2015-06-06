#ifndef INCLUDED_MQF_AABB
#define INCLUDED_MQF_AABB
#include <Eigen/Core>
#include "../utility.h"

namespace mqf {

	template<typename T,int N>
	struct AABB {
		using Vec = Eigen::Matrix<T,N,1>;
		
		Vec lower, upper;

		AABB() = default;
		
		AABB( const Vec& lower,
		      const Vec& upper ) :
			minBounds( lower ),
			maxBounds( upper )
		{}

		auto vector() const {
			return upper - lower;
		}

	};

	using AABB1 = AABB<double,1>;
	using AABB2 = AABB<double,2>;
	using AABB3 = AABB<double,3>;
	using AABB4 = AABB<double,4>;

	template<typename T,int N>
	void clamp( Eigen::Matrix<T,N,1>& x, const AABB<T,N>& bounds ) {
		for(int i=0;i<N;++i) {
			x[i] = clamp( x[i], bounds.lower[i], bounds.upper[i] );
		}
	}

}

#endif
