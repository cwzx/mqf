#ifndef INCLUDED_MQF_TRADING_RESULTS
#define INCLUDED_MQF_TRADING_RESULTS
#include "../stats/descriptive.h"
#include "return.h"
#include "../time_series/drawdown.h"
#include <iostream>

namespace mqf {

	struct TestResult {
		double annualLogReturn,
		       annualVolatility,
		       sharpeRatio;
		DrawDown maxDrawDown;

		void print() const {
			using namespace std;
			cout << "Annual Log Return: " << annualLogReturn                        << endl;
			cout << "Annual Return: "     << returnFromLogReturn( annualLogReturn ) << endl;
			cout << "Annual Volatility: " << annualVolatility                       << endl;
			cout << "Sharpe Ratio: "      << sharpeRatio                            << endl;
			cout << "Max Drawdown: "      << maxDrawDown.lossFraction()             << endl;
		}

	};

	inline double sharpeRatio( double ret, double vol, double riskFreeRate ) {
		return ( ret - riskFreeRate ) / vol;
	}

	template<typename It>
	TestResult computeTestResults( It p1, It p2, double dt, double riskFreeRate = 0 ) {
		auto returns = computeLogReturns( p1, p2 );
		auto mean = sampleMean( returns.begin(), returns.end() );
		auto var = sampleVariance( returns.begin(), returns.end(), mean );
		
		HistogramGenerator().generate( returns.begin(), returns.end() ).writeCSV("hist.csv");

		TestResult res;
		res.annualLogReturn = mean / dt;
		res.annualVolatility = std::sqrt( var / dt );
		res.sharpeRatio = sharpeRatio( res.annualLogReturn, res.annualVolatility, riskFreeRate );
		res.maxDrawDown = maxDrawDown(p1,p2);
		return res;
	}

}

#endif
