#ifndef INCLUDED_MQF_OPTIMIZATION_BRUTE_FORCE
#define INCLUDED_MQF_OPTIMIZATION_BRUTE_FORCE
#include <cstdint>
#include <cmath>
#include "../utility.h"
#include "../pde/aabb.h"
#include "../pde/grid.h"
#include "../stats/histogram.h"
#include <Eigen/Core>

namespace mqf {

	template<typename T,int N>
	struct BruteForce {
		using Point = Eigen::Matrix<T,N,1>;
		AABB<T,N> bounds;
		Grid<uint32_t,N> grid;

		struct Results;

		BruteForce() {
			grid.size.setConstant(100);
			bounds.minBounds.setZero();
			bounds.maxBounds.setOnes();
		}

		auto value( const MultiIndex<uint32_t,N>& it ) const {
			return bounds.minBounds + (( bounds.vector().array() / ( grid.size - MultiIndex<uint32_t,N>::Ones() ).cast<double>() ) * it.cast<double>()).matrix();
		}

		template<typename S>
		Results optimize( S&& cost ) const {
			auto count = grid.size.prod();
			std::vector<T> results;
			results.reserve( count );

			auto it = grid.begin();
			auto end = grid.end();

			Results R;
			R.optimal = value(*it);
			R.cost = cost( R.optimal );
			results.push_back( R.cost );
			for(++it;it!=end;++it) {
				auto x = value(*it);
				auto c = cost( x );
				results.push_back( c );
				if( c < R.cost ) {
					R.cost = c;
					R.optimal = x;
				}
			}

			R.histogram = HistogramGenerator().generate( results.begin(), results.end() );
			return R;
		}

		template<typename S>
		Results maximize( S&& cost ) const {
			return optimize( [&](auto&& x){ return -cost(x); } );
		}

	};

	template<typename T,int N>
	struct BruteForce<T,N>::Results {
		Point optimal;
		double cost;
		Histogram histogram;
	};
	
}

#endif
