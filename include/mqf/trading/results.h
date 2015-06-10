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
		       sharpeRatio,
		       sortinoRatio,
		       calmarRatio;
		DrawDown maxDrawDown;

		void print( std::ostream& os ) const {
			os << "Annual Return: "     << returnFromLogReturn( annualLogReturn ) << '\n'
			   << "Annual Log Return: " << annualLogReturn                        << '\n'
			   << "Annual Volatility: " << annualVolatility                       << '\n'
			   << "Sharpe Ratio: "      << sharpeRatio                            << '\n'
			   << "Max Drawdown: "      << maxDrawDown.lossFraction()             << '\n'
			   << "Sortino Ratio: "     << sortinoRatio                           << '\n'
			   << "Calmar Ratio: "      << calmarRatio                            << '\n';
		}

		void print() const {
			print( std::cout );
		}

	};

	inline double sharpeRatio( double ret, double vol, double riskFreeRate ) {
		if( vol == 0.0 ) return 0.0;
		return ( ret - riskFreeRate ) / vol;
	}

	inline double sortinoRatio( double ret, double downsideDeviation, double riskFreeRate ) {
		if( downsideDeviation == 0.0 ) return 0.0;
		return ( ret - riskFreeRate ) / downsideDeviation;
	}

	inline double calmarRatio( double ret, double mdd, double riskFreeRate ) {
		if( mdd == 0.0 ) return 0.0;
		return ( ret - riskFreeRate ) / mdd;
	}

	inline double sharpeRatioVariance( double sharpe, double T ) {
		return (1.0 + 0.5 * sharpe * sharpe ) / std::sqrt( T );
	}

	template<typename It>
	TestResult computeTestResults( It p1, It p2, double dt, double riskFreeRate = 0 ) {
		auto returns = computeLogReturns( p1, p2 );
		auto mean = sampleMean( returns.begin(), returns.end() );
		auto var = sampleVariance( returns.begin(), returns.end(), mean );
		auto dsVar = downsideVariance( returns.begin(), returns.end(), mean );

		TestResult res;
		res.annualLogReturn = mean / dt;
		res.annualVolatility = std::sqrt( var / dt );
		res.sharpeRatio = sharpeRatio( res.annualLogReturn, res.annualVolatility, riskFreeRate );
		res.maxDrawDown = maxDrawDown(p1,p2);
		res.sortinoRatio = sortinoRatio( res.annualLogReturn, std::sqrt( dsVar / dt ), riskFreeRate );
		res.calmarRatio = calmarRatio( res.annualLogReturn, res.maxDrawDown.logLoss(), riskFreeRate );
		return res;
	}

}

#endif
