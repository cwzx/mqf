#ifndef INCLUDED_MQF_FOKKERPLANCK
#define INCLUDED_MQF_FOKKERPLANCK
#include <valarray>
#include <cstdint>

using std::valarray;

namespace mqf {

	valarray<double> div( const valarray<double>& x, double dx = 1.0 ) {
		auto N = x.size();
		valarray<double> r(N);
		double boundary = 0.0;
		double r2dx = 1.0 / (2.0 * dx);

		r[0] = ( x[1] - boundary ) * r2dx;
		for(uint32_t i=1;i<N-1;++i) {
			r[i] = ( x[i+1] - x[i-1] ) * r2dx;
		}
		r[N-1] = ( boundary - x[N-2] ) * r2dx;

		return r;
	}

	valarray<double> laplacian( const valarray<double>& x, double dx = 1.0 ) {
		auto N = x.size();
		valarray<double> r(N);
		double boundary = 0.0;
		double rdx2 = 1.0 / (dx * dx);

		r[0] = ( x[1] - 2.0 * x[0] + boundary ) * rdx2;
		for(uint32_t i=1;i<N-1;++i) {
			r[i] = ( x[i+1] - 2.0 * x[i] + x[i-1] ) * rdx2;
		}
		r[N-1] = ( boundary - 2.0 * x[N-1] + x[N-2] ) * rdx2;

		return r;
	}

	template<typename Drift,typename Diffusion,typename Tag>
	struct FokkerPlanck;

	template<typename Drift,typename Diffusion,typename Tag>
	struct FokkerPlanck<Drift,Diffusion,ItoDiffusionTag> {
		using Time = double;
		using State = valarray<double>;

		Drift drift;
		Diffusion diffusion;

		State operator()( const State& f, Time dt ) {
			double mu = drift();
			double sigma = diffusion();
			return -div( f * mu ) + 0.5 * laplacian( f * sigma*sigma );
		}
	};

	template<typename Drift,typename Diffusion,typename Tag>
	struct FokkerPlanck<Drift,Diffusion,ItoProcessTag> {
		using Time = double;
		using State = valarray<double>;

		Drift drift;
		Diffusion diffusion;

		State operator()( const State& f, Time t, Time dt ) {
			auto mu = drift(t);
			auto sigma = diffusion(t);
			return -div( f * mu ) + 0.5 * laplacian( f * sigma*sigma );
		}

	};

}

#endif
