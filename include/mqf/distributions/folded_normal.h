#ifndef INCLUDED_MQF_DISTRIBUTIONS_FOLDED_NORMAL
#define INCLUDED_MQF_DISTRIBUTIONS_FOLDED_NORMAL
#include "normal.h"
#include "../functions.h"

namespace mqf {
namespace Distributions {

	struct FoldedNormal : Density<> {
		double mu, sigma2;

		FoldedNormal( double mu = 0.0, double sigma2 = 1.0 ) : mu(mu), sigma2(sigma2) {}

		double mean() const {
			return std::sqrt( (2.0 / Pi) * sigma2 ) * std::exp(-0.5 * mu*mu / sigma2)
			     + mu * std::erf( -mu / std::sqrt(2.0 * sigma2) );
		}

		double variance() const {
			double y = mean();
			return mu*mu * sigma2 - y*y;
		}

		double operator()( double x ) const {
			if( x < 0.0 ) return 0.0;
			auto a = -0.5 / sigma2;
			return ( std::exp( a * square(x-mu) ) + std::exp( a * square(x+mu) ) )
			     / std::sqrt( 2.0 * Pi * sigma2 );
		}

		double derivative( double x ) const {
			if( x < 0.0 ) return 0.0;
			auto xpm = x + mu;
			auto xmm = x - mu;
			auto a = -0.5 / sigma2;
			return -( std::exp( a * square(xmm) ) * xmm + std::exp( a * square(xpm) ) * xpm )
			     / ( sigma2 * std::sqrt( 2.0 * Pi * sigma2 ) );
		}

		double cumulative( double x ) const {
			if( x <= 0.0 ) return 0.0;
			double c = 1.0 / std::sqrt( 2.0 * sigma2 );
			return 0.5 * ( std::erf( c*(x+mu) ) + std::erf( c*(x-mu) ) );
		}

	};

	struct HalfNormal : Density<> {
		double sigma2;

		HalfNormal( double sigma2 = 1.0 ) : sigma2(sigma2) {}

		double mean() const {
			return std::sqrt( (2.0 / Pi)  * sigma2 );
		}
		
		double median() const {
			return std::sqrt( 2.0 * sigma2 ) * InvErfHalf;
		}

		double variance() const {
			return sigma2 * (1.0 - 2.0/Pi);
		}

		double operator()( double x ) const {
			if( x < 0.0 ) return 0.0;
			return std::exp( -0.5 * x*x / sigma2 ) * std::sqrt( 2.0 / (Pi * sigma2) );
		}

		double derivative( double x ) const {
			if( x < 0.0 ) return 0.0;
			return -x * std::exp( -0.5 * x*x / sigma2 ) / ( sigma2 * std::sqrt( (0.5 * Pi) * sigma2 ) );
		}

		double cumulative( double x ) const {
			if( x <= 0.0 ) return 0.0;
			return std::erf( x / std::sqrt( 2.0 * sigma2 ) );
		}

		double entropy( double x ) const {
			if( x <= 0.0 ) return 0.0;
			return 0.5 * ( 1.0 + std::log( (0.5 * Pi) * sigma2 ) );
		}

		operator FoldedNormal() const {
			return FoldedNormal( 0.0, sigma2 );
		}

	};

	struct StdHalfNormal : Density<> {

		double mean() const {
			return std::sqrt( 2.0 / Pi );
		}
		
		double median() const {
			return Sqrt2 * InvErfHalf;
		}

		double variance() const {
			return 1.0 - 2.0/Pi;
		}

		double operator()( double x ) const {
			if( x < 0.0 ) return 0.0;
			return std::exp( -0.5 * x*x ) * std::sqrt( 2.0 / Pi );
		}

		double derivative( double x ) const {
			if( x < 0.0 ) return 0.0;
			return -x * std::exp( -0.5 * x*x ) / std::sqrt( 0.5 * Pi );
		}

		double cumulative( double x ) const {
			if( x <= 0.0 ) return 0.0;
			return std::erf( x / Sqrt2 );
		}

		double entropy( double x ) const {
			if( x <= 0.0 ) return 0.0;
			return 0.5 * ( 1.0 + std::log( 0.5 * Pi ) );
		}

		operator FoldedNormal() const {
			return FoldedNormal( 0.0, 1.0 );
		}

		operator HalfNormal() const {
			return HalfNormal( 1.0 );
		}

	};

	RV<FoldedNormal> abs( const RV<Normal>& x ) {
		return MakeRV( FoldedNormal( x.dist.mu, x.dist.sigma2 ) );
	}

	RV<StdHalfNormal> abs( RV<StdNormal> ) {
		return MakeRV( StdHalfNormal() );
	}

}
}

#endif
