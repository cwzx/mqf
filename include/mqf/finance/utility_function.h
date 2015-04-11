#ifndef INCLUDED_MQF_UTILITY_FUNCTION
#define INCLUDED_MQF_UTILITY_FUNCTION
#include <cmath>

namespace mqf {
namespace Utility {

	struct Log {
		double operator()( double x ) const {
			return std::log(x);
		}
	};

	struct Exp {
		double alpha = 1.0;

		Exp() = default;
		explicit Exp( double alpha ) : alpha(alpha) {}

		double operator()( double x ) const {
			if( alpha == 0.0 ) {
				return x;
			}
			return ( 1.0 - std::exp( -alpha * x ) ) / alpha;
		}
	};

	struct IsoElastic {
		double eta = 1.0;

		IsoElastic() = default;
		explicit IsoElastic( double eta ) : eta(eta) {}

		double operator()( double x ) const {
			if( eta == 1.0 ) {
				return std::log(x);
			}
			return ( std::pow( x, 1.0 - eta ) - 1.0 ) / ( 1.0 - eta );
		}
	};

}

	template<typename Utility>
	struct AbsoluteRiskAversion;

	template<typename Utility>
	struct RelativeRiskAversion {
		double operator()( double x ) const {
			return x * AbsoluteRiskAversion<Utility>()( x );
		}
	};

	template<>
	struct AbsoluteRiskAversion<Utility::Log> {
		double operator()( double x ) const {
			return 1.0 / x;
		}
	};

	template<>
	struct RelativeRiskAversion<Utility::Log> {
		double operator()( double x ) const {
			return 1.0;
		}
	};

	template<>
	struct AbsoluteRiskAversion<Utility::Exp> {
		double alpha = 1.0;

		AbsoluteRiskAversion() = default;
		explicit AbsoluteRiskAversion( double alpha ) : alpha(alpha) {}

		double operator()( double ) const {
			return alpha;
		}
	};

	template<>
	struct RelativeRiskAversion<Utility::Exp> {
		double alpha = 1.0;

		RelativeRiskAversion() = default;
		explicit RelativeRiskAversion( double alpha ) : alpha(alpha) {}

		double operator()( double x ) const {
			return alpha * x;
		}
	};

	template<>
	struct AbsoluteRiskAversion<Utility::IsoElastic> {
		double eta = 1.0;

		AbsoluteRiskAversion() = default;
		explicit AbsoluteRiskAversion( double eta ) : eta(eta) {}

		double operator()( double x ) const {
			return eta / x;
		}
	};

	template<>
	struct RelativeRiskAversion<Utility::IsoElastic> {
		double eta = 1.0;

		RelativeRiskAversion() = default;
		explicit RelativeRiskAversion( double eta ) : eta(eta) {}

		double operator()( double ) const {
			return eta;
		}
	};


}

#endif
