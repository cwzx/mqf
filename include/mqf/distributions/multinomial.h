#ifndef INCLUDED_MQF_DISTRIBUTIONS_MULTINOMIAL
#define INCLUDED_MQF_DISTRIBUTIONS_MULTINOMIAL
#include <initializer_list>
#include "../distribution.h"
#include "../functions.h"

namespace mqf {
namespace Distributions {

	template<uint32_t k>
	struct Multinomial : Mass<uint32_t> {
		double p[k];
		uint32_t n;

		template<typename Seq>
		Multinomial( uint32_t n, const Seq& pr ) : n(n) {
			for(uint32_t i=0;i<k;++i) {
				p[i] = pr[i];
			}
		}
		
		template<typename T>
		Multinomial( uint32_t n, const std::initializer_list<T>& list ) : Multinomial(n,list.begin()) {}

		double mean( uint32_t i ) const {
			return n * p[i];
		}

		double covariance( uint32_t i, uint32_t j ) const {
			return n * p[i] * ( delta(i,j) - p[j] );
		}

		double variance( uint32_t i ) const {
			return n * p[i]*(1.0-p[i]);
		}

		template<typename T>
		double operator()( const std::initializer_list<T>& list ) const {
			return (*this)( list.begin() );
		}

		template<typename Seq>
		double operator()( const Seq& x ) const {
			double result = logFactorial( n );
			for(uint32_t i=0;i<k;++i) {
				result += x[i] * std::log( p[i] ) - logFactorial( x[i] );
			}
			return std::exp( result );
		}

	};

	using Trinomial = Multinomial<3>;

}
}

#endif
