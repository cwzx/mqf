#ifndef INCLUDED_MQF_TRADING_RESULTS
#define INCLUDED_MQF_TRADING_RESULTS
#include "../stats/descriptive.h"
#include "return.h"
#include "../time_series/drawdown.h"
#include <iostream>
#include "../stats/kde.h"
#include "../stats/kernels.h"

namespace mqf {

	struct TestResult {
		double annualLogReturn,
		       annualVolatility,
		       sharpeRatio;
		DrawDown maxDrawDown;

		void print( std::ostream& os ) const {
			using std::endl;
			os << "Annual Log Return: " << annualLogReturn                        << endl;
			os << "Annual Return: "     << returnFromLogReturn( annualLogReturn ) << endl;
			os << "Annual Volatility: " << annualVolatility                       << endl;
			os << "Sharpe Ratio: "      << sharpeRatio                            << endl;
			os << "Max Drawdown: "      << maxDrawDown.lossFraction()             << endl;
		}

		void print() const {
			print( std::cout );
		}

	};

	inline double sharpeRatio( double ret, double vol, double riskFreeRate ) {
		if( vol == 0.0 ) return 0.0;
		return ( ret - riskFreeRate ) / vol;
	}

	template<typename It>
	TestResult computeTestResults( It p1, It p2, double dt, double riskFreeRate = 0 ) {
		auto returns = computeLogReturns( p1, p2 );
		auto mean = sampleMean( returns.begin(), returns.end() );
		auto var = sampleVariance( returns.begin(), returns.end(), mean );

		TestResult res;
		res.annualLogReturn = mean / dt;
		res.annualVolatility = std::sqrt( var / dt );
		res.sharpeRatio = sharpeRatio( res.annualLogReturn, res.annualVolatility, riskFreeRate );
		res.maxDrawDown = maxDrawDown(p1,p2);
		return res;
	}

}

#endif
