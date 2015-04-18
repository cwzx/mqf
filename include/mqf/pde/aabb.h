#ifndef INCLUDED_MQF_AABB
#define INCLUDED_MQF_AABB
#include <Eigen/Core>

namespace mqf {

	template<typename T,int N>
	struct AABB {
		using Vec = Eigen::Matrix<T,N,1>;
		
		Vec minBounds, maxBounds;

		AABB( const Vec& minBounds,
		      const Vec& maxBounds ) :
			minBounds( minBounds ),
			maxBounds( maxBounds )
		{}

	};

	using AABB1 = AABB<double,1>;
	using AABB2 = AABB<double,2>;
	using AABB3 = AABB<double,3>;
	using AABB4 = AABB<double,4>;

}

#endif
