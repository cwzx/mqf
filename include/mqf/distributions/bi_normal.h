#ifndef INCLUDED_MQF_DISTRIBUTIONS_BI_NORMAL
#define INCLUDED_MQF_DISTRIBUTIONS_BI_NORMAL
#include <cmath>
#include "../distribution.h"
#include "../random_variable.h"
#include "../constants.h"
#include <Eigen/Core>

namespace mqf {
namespace Distributions {

	struct BiStdNormal : Density<> {
		using Vec = Eigen::Vector2d;
		double rho;

		explicit BiStdNormal( double rho = 0.0 ) : rho(rho) {}

		Vec mean() const {
			return Vec::Zero();
		}

		Vec mode() const {
			return Vec::Zero();
		}

		double operator()( double x, double y ) const {
			double r = 1.0 - rho*rho;
			return std::exp( -0.5 * (x*x + y*y - 2.0*rho*x*y) / r ) / ( 2.0 * Pi * std::sqrt(r) );
		}

		Vec derivative( double x, double y ) const {
			double r = 1.0 - rho*rho;
			Vec w;
			w[0] = rho * y - x;
			w[1] = rho * x - y;
			return w * ( (*this)(x,y) / r );
		}

		double cumulative( double x, double y ) const {
			//static_assert(false,"Not implemented");
			return 0.0;
		}
		
		double log( double x, double y ) const {
			double r = 1.0 - rho*rho;
			return -0.5 * (x*x + y*y - 2.0*rho*x*y) / r - std::log( 2.0 * Pi ) - 0.5 * std::log( r );
		}

		double operator()( Vec v ) const {
			return (*this)( v[0], v[1] );
		}

		Vec derivative( Vec v ) const {
			return derivative( v[0], v[1] );
		}

		double log( Vec v ) const {
			return log( v[0], v[1] );
		}

	};

}
}

#endif

