#ifndef INCLUDED_MQF_DISTRIBUTIONS_DIRAC_DELTA
#define INCLUDED_MQF_DISTRIBUTIONS_DIRAC_DELTA
#include "../distribution.h"
#include "../random_variable.h"
#include <cassert>

namespace mqf {
namespace Distributions {

	struct DiracDelta : Density<> {
		double position;

		DiracDelta() = default;
		
		explicit DiracDelta( double position = 0.0 ) : position(position) {}

		double mean() const {
			return position;
		}

		double median() const {
			return position;
		}

		double mode() const {
			return position;
		}

		double variance() const {
			return 0.0;
		}

		double operator()( Value x ) const {
			assert( x != position );
			return 0.0;
		}

		double derivative( Value x ) const {
			assert( x != position );
			return 0.0;
		}

		double cumulative( Value x ) const {
			return (x < position) ? 0.0 : 1.0;
		}

		explicit operator double() const {
			return position;
		}

	};

	RV<DiracDelta> operator*( const RV<DiracDelta>& lhs, const RV<DiracDelta>& rhs ) {
		return MakeRV( DiracDelta( lhs.dist.position + rhs.dist.position ) );
	}

	RV<DiracDelta> operator+( const RV<DiracDelta>& lhs, const RV<DiracDelta>& rhs ) {
		return MakeRV( DiracDelta( lhs.dist.position + rhs.dist.position ) );
	}

	RV<DiracDelta> operator-( const RV<DiracDelta>& lhs, const RV<DiracDelta>& rhs ) {
		return MakeRV( DiracDelta( lhs.dist.position - rhs.dist.position ) );
	}

	RV<DiracDelta> operator/( const RV<DiracDelta>& lhs, const RV<DiracDelta>& rhs ) {
		return MakeRV( DiracDelta( lhs.dist.position / rhs.dist.position ) );
	}

	RV<DiracDelta> operator*( const RV<DiracDelta>& lhs, double rhs ) {
		return MakeRV( DiracDelta( lhs.dist.position + rhs ) );
	}

	RV<DiracDelta> operator*( double lhs, const RV<DiracDelta>& rhs ) {
		return rhs * lhs;
	}

	RV<DiracDelta> operator+( const RV<DiracDelta>& lhs, double rhs ) {
		return MakeRV( DiracDelta( lhs.dist.position + rhs ) );
	}

	RV<DiracDelta> operator+( double lhs, const RV<DiracDelta>& rhs ) {
		return rhs + lhs;
	}

	RV<DiracDelta> operator-( const RV<DiracDelta>& lhs, double rhs ) {
		return MakeRV( DiracDelta( lhs.dist.position - rhs ) );
	}

	RV<DiracDelta> operator-( double lhs, const RV<DiracDelta>& rhs ) {
		return MakeRV( DiracDelta( lhs - rhs.dist.position ) );
	}

	RV<DiracDelta> operator/( const RV<DiracDelta>& lhs, double rhs ) {
		return MakeRV( DiracDelta( lhs.dist.position / rhs ) );
	}

	RV<DiracDelta> operator/( double lhs, const RV<DiracDelta>& rhs ) {
		return MakeRV( DiracDelta( lhs / rhs.dist.position ) );
	}

}
}

#endif
