#ifndef INCLUDED_MQF_DATA_STOCKS
#define INCLUDED_MQF_DATA_STOCKS
#include "../gregorian.h"
#include <vector>

namespace mqf {

	struct Dividend {
		Gregorian::Date date;
		double amountPerShare;
	};

	struct Split {
		Gregorian::Date date;
		double factor;
	};

	struct StockData {
		using Date = Gregorian::Date;

		// daily price data
		std::vector<Date> dates;
		std::vector<double> open, high, low, close;
		std::vector<uint64_t> volume;

		std::vector<Split> splits;
		std::vector<Dividend> dividends;

		double computeSplitFactor( const Date& from, const Date& to ) const {
			double factor = 1.0;
			for( auto&& s : splits ) {
				if( s.date > from ) factor *= s.factor;
				if( s.date > to ) break;
			}
			return factor;
		}

		Date firstDate() const {
			return dates.front();
		}

		Date lastDate() const {
			return dates.back();
		}

		std::vector<double> computeAdjustedClose() const {
			auto N = close.size();
			std::vector<double> adj;
			adj.reserve( N );
			double factor = 1.0;
			auto nextSplit = splits.begin();
			auto endSplit = splits.end();
			for(size_t i=0;i<N;++i) {
				if( nextSplit != endSplit && dates[i] == nextSplit->date ) {
					factor *= nextSplit->factor;
					++nextSplit;
				}
				adj.push_back( close[i] * factor );
			}
			return adj;
		}

	};

}

#endif
