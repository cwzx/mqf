#ifndef INCLUDED_MQF_DISTRIBUTIONS_STUDENTS_T
#define INCLUDED_MQF_DISTRIBUTIONS_STUDENTS_T
#include <cassert>
#include <cmath>
#include <limits>
#include <random>
#include "../distribution.h"
#include "../constants.h"
#include "../functions.h"

namespace mqf {
namespace Distributions {

	struct StudentsT : Density<> {
		double nu;

		explicit StudentsT( double dof = 0.0 ) : nu(dof) {}

		double mean() const {
			assert( nu > 1 );
			return 0;
		}

		double median() const {
			return 0;
		}

		double mode() const {
			return 0;
		}

		double variance() const {
			assert( nu > 2 );
			return nu / ( nu - 2.0 );
		}

		double skewness() const {
			assert( nu > 3 );
			return 0.0;
		}

		double excessKurtosis() const {
			assert( nu > 2 );
			if( nu > 4.0 )
				return 6.0 / (nu - 4.0);
			return std::numeric_limits<double>::infinity();
		}

		double operator()( double x ) const {
			double p = (nu + 1.0) * 0.5;
			return std::tgamma( p ) / ( std::sqrt(nu * Pi) * std::tgamma( 0.5 * nu ) ) * std::pow( 1.0 + x*x / nu, -p );
		}

		std::student_t_distribution<Value> distribution() const {
			return std::student_t_distribution<Value>( nu );
		}

	};
	
	struct StudentsTLS : Density<> {
		double nu, mu, sigma2;

		explicit StudentsTLS( double dof = 0.0,
		                      double mean = 0.0,
		                      double scale = 1.0 ) :
			nu( dof ),
			mu( mean ),
			sigma2( scale )
		{}

		double mean() const {
			assert( nu > 1 );
			return mu;
		}

		double median() const {
			return mu;
		}

		double mode() const {
			return mu;
		}

		double variance() const {
			assert( nu > 2 );
			return sigma2 * nu / ( nu - 2.0 );
		}
		
		double skewness() const {
			assert( nu > 3 );
			return 0.0;
		}

		double excessKurtosis() const {
			assert( nu > 2 );
			if( nu > 4.0 )
				return 6.0 / (nu - 4.0);
			return std::numeric_limits<double>::infinity();
		}

		double operator()( double x ) const {
			double p = (nu + 1.0) * 0.5;
			return std::tgamma( p ) / ( std::sqrt(nu * Pi * sigma2) * std::tgamma( 0.5 * nu ) * std::pow( 1.0 + square(x-mu) / (nu * sigma2), p ) );
		}

		double derivative( double x ) const {
			auto y = x - mu;
			return -std::tgamma( 0.5*(nu + 3.0) ) * y / ( std::tgamma( 0.5*(nu + 2.0) ) * sigma2 * std::sqrt( Pi * nu * sigma2 * std::pow( y*y/(nu*sigma2) + 1.0, nu + 3.0 ) ) );
		}

	};

}

	template<typename>
	struct MomentEstimation;

	template<>
	struct MomentEstimation<Distributions::StudentsT> {
		using Dist = Distributions::StudentsT;
		template<typename It>
		Dist operator()( It p1, It p2 ) const {
			auto var = sampleVariance(p1,p2);
			return Dist( 2.0 * var / (var - 1.0) );
		}
	};

	template<>
	struct MomentEstimation<Distributions::StudentsTLS> {
		using Dist = Distributions::StudentsTLS;

		template<typename It>
		Dist operator()( It p1, It p2 ) const {
			auto mu = sampleMean(p1,p2);
			auto var = sampleVariance(p1,p2,mu);
			auto k = sampleExKurtosis(p1,p2,mu);
			auto nu = 6.0 / k + 4.0;
			return Dist( nu, mu, var * (nu - 2.0)/nu );
		}

		template<typename It>
		Dist operator()( It p1, It p2, double nu ) const {
			auto mu = sampleMean(p1,p2);
			auto var = sampleVariance(p1,p2,mu);
			return Dist( nu, mu, var * (nu - 2.0)/nu );
		}
	};

}

#endif
