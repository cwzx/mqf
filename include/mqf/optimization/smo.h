#ifndef INCLUDED_MQF_OPTIMIZATION_SMO
#define INCLUDED_MQF_OPTIMIZATION_SMO
#include <cmath>
#include <Eigen/Core>
#include <tuple>
#include <utility>
#include <limits>
#include "../utility.h"

namespace mqf {

	/*
	 * Sequential Minimal Optimization
	 *
	 * Uses WSS3 and Algorithm2 from Fan et. al, Journal of Machine Learning Research 6 (2005) 1889--1918
	 *
	 */
	struct SMO {
		using Veci = Eigen::Matrix<int8_t,-1,1>;
		using Vec = Eigen::VectorXd;
		using Mat = Eigen::MatrixXd;
		
		Veci y;
		Mat Q;

		double C = 1.0,
		       tolerance = 1.0e-3;
		int max_its = 1000;

		static constexpr double tau = 1.0e-12;

		template<typename It>
		void setY( It p1, It p2 ) {
			auto N = std::distance(p1,p2);
			y.resize(N);
			for( int i=0; p1 != p2; ++i,++p1 ) {
				y[i] = *p1;
			}
		}

		template<typename It,typename Kernel>
		void computeQ( It px1, It px2, Kernel&& kernel ) {
			auto N = std::distance(px1,px2);
			Q.resize(N,N);
			for( int j=0, auto xj = px1; xj != px2; ++j,++xj ) {
				for( int i=j+1, auto xi = std::next(xj); xi != px2; ++i,++xi ) {
					Q(i,j) = y[i] * y[j] * std::forward<Kernel>(kernel)( *xi, *xj );
				}
			}
		}

		std::pair<int,int> workingSetSelection( const Vec& gradient, const Vec& alpha ) const {
			
			auto N = y.size();

			// select i
			auto i = -1;
			auto G_max = -std::numeric_limits<double>::infinity();
			auto G_min = std::numeric_limits<double>::infinity();
			for(int t=0;t<N;++t) {
				if( (y[t] == 1 && alpha[t] < C) || (y[t] == -1 && alpha[t] > 0) ) {
					auto Gr = -y[t] * gradient[t];
					if( Gr >= G_max ) {
						i = t;
						G_max = Gr;
					}
				}
			}

			// select j
			auto j = -1;
			auto obj_min = std::numeric_limits<double>::infinity();
			for(int t=0;t<N;++t) {
				if( (y[t] == 1 && alpha[t] > 0) || (y[t] == -1 && alpha[t] < C) ) {
					auto Gr = -y[t] * gradient[t];
					if( Gr <= G_min )
						G_min = Gr;

					auto b = G_max - Gr;
					if( b > 0 ) {
						auto a = Q(i,i) + Q(t,t) - 2.0 * y[i] * y[t] * Q(i,t);
						if( a <= 0 )
							a = tau;
						auto obj = -b*b/a;
						if( obj <= obj_min ) {
							j = t;
							obj_min = obj;
						}
					}
				}
			}
			if( G_max - G_min < tolerance )
				return std::make_pair(-1,-1);

			return std::make_pair(i,j);
		}
		
		Vec solve() const {
			Vec alpha, gradient;
			alpha.setZero( y.size() );
			gradient.setConstant( y.size(), -1.0 );

			for(int k=0;k<max_its;++k) {

				int i, j;
				std::tie(i,j) = workingSetSelection( gradient, alpha );

				if( j == -1 )
					break;

				// read relevant quantities for (i,j)
				auto yi = y[i]; auto ai = alpha[i]; auto gi = gradient[i];
				auto yj = y[j]; auto aj = alpha[j]; auto gj = gradient[j];

				auto Qii = Q(i,i); auto Qjj = Q(j,j); auto Qij = Q(i,j);

				auto a = Qii + Qjj - 2.0 * yi * yj * Qij;
				auto b = -yi * gi + yj * gj;

				if( a <= 0 ) a = tau;

				// update alpha
				auto oldAi = ai;
				auto oldAj = aj;
				ai += yi * (b/a);
				aj -= yj * (b/a);

				// project alpha back to the feasible region
				auto sum = yi * oldAi + yj * oldAj;

				ai = clamp( ai, 0.0, C );
				aj = yj * ( sum - yi * ai );
				aj = clamp( aj, 0.0, C );
				ai = yi * ( sum - yj * aj );

				// update gradient
				gradient += Q.col(i) * (ai - oldAi) + Q.col(j) * (aj - oldAj);
				
				// write new alphas
				alpha[i] = ai;
				alpha[j] = aj;
			}

			return alpha;
		}

	};

}

#endif
