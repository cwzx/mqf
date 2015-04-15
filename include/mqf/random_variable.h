#ifndef INCLUDED_MQF_RANDOM_VARIABLE
#define INCLUDED_MQF_RANDOM_VARIABLE

namespace mqf {

	template<typename Dist>
	struct RV {
		using Value = typename Dist::Value;

		Dist dist;

		explicit RV( const Dist& d ) : dist(d) {}
	};

	template<typename Dist>
	RV<Dist> MakeRV( const Dist& d ) {
		return RV<Dist>( d );
	}

	template<typename Dist>
	typename Dist::Value Expectation( const RV<Dist>& X ) {
		return X.dist.mean();
	}

	template<typename Dist>
	typename Dist::Value Variance( const RV<Dist>& X ) {
		return X.dist.variance();
	}

}

#endif
