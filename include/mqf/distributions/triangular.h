#ifndef INCLUDED_MQF_DISTRIBUTIONS_TRIANGULAR
#define INCLUDED_MQF_DISTRIBUTIONS_TRIANGULAR
#include "../random_variable.h"
#include "../distribution.h"
#include <cmath>

namespace mqf {
namespace Distributions {

	struct Triangular : Density<> {
		double a,b,c;
		
		explicit Triangular( double lower = 0.0,
		                     double upper = 1.0,
		                     double mode = 0.5 ) :
			a(lower),
			b(upper),
			c(mode)
		{}

		double mean() const {
			return (a + b + c) / 3.0;
		}

		double median() const {
			double d = 0.5*(a+b);
			if( c >= d )
				return a + std::sqrt( 0.5*(b-a)*(c-a) );
			return b - std::sqrt( 0.5*(b-a)*(b-c) );
		}

		double mode() const {
			return c;
		}

		double variance() const {
			return ( a*a + b*b + c*c - a*b - a*c - b*c ) / 18.0;
		}

		double excessKurtosis() const {
			return -0.6;
		}

		double operator()( double x ) const {
			if( x <= a || b <= x )
				return 0.0;
			double p = 2.0/(b-a);
			if( x <= c )
				return p * (x-a)/(c-a);
			return p * (b-x)/(b-c);
		}

		double derivative( double x ) const {
			if( x <= a || b <= x )
				return 0.0;
			double p = (2.0*x)/(b-a);
			if( x <= c )
				return p/(c-a);
			return -p/(b-c);
		}

		double cumulative( double x ) const {
			if( x <= a )
				return 0.0;
			if( b <= x )
				return 1.0;
			if( x <= c )
				return ((x-a)*(x-a))/((b-a)*(c-a));
			return 1.0 - ((b-x)*(b-x))/((b-a)*(b-c));
		}

		std::piecewise_linear_distribution<Value> distribution() const {
			double xs[] = { a, c, b };
			double ws[] = { 0, 1, 0 };
			return std::piecewise_linear_distribution<Value>( std::begin(xs), std::end(xs), std::begin(ws) );
		}

	};

}
}

#endif
