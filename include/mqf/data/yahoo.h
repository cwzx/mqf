#ifndef INCLUDED_MQF_DATA_YAHOO
#define INCLUDED_MQF_DATA_YAHOO
#include "../gregorian.h"
#include <vector>
#include <fstream>

/*
 * Parsing stock price data from Yahoo Finance
 *
 */
namespace mqf {
namespace Yahoo {

	/*
	 * Parse date of the form "yyyy-mm-dd", e.g. "2015-04-27"
	 *
	 */
	Gregorian::Date parseDate( char* str ) {
		Gregorian::Date date;
		
		str[ 4] = '\0';
		str[ 7] = '\0';
		str[10] = '\0';

		date.year = atoi( &str[0] );
		date.month = Gregorian::Month( atoi( &str[5] ) );
		date.day = atoi( &str[8] );

		return date;
	}
	
	struct DailyData {
		Gregorian::Date date;
		double open, high, low, close, adj_close;
		uint64_t volume;
	};

	/*
	 * Load .csv files produced by real-chart.finance.yahoo.com/table.csv
	 *
	 */
	std::vector<DailyData> load( const char* file ) {
		std::vector<DailyData> timeseries;
		std::ifstream in(file);
		if( !in ) return timeseries;

		const int buf_size = 16;
		char buf[buf_size];

		// skip over the header row
		in.ignore( std::numeric_limits<std::streamsize>::max(), '\n');

		while( !(in.peek(),in.eof()) ) {
			DailyData daily;
			
			in.getline(buf,buf_size,',');
			daily.date = parseDate( buf );

			in.getline(buf,buf_size,',');
			daily.open = atof(buf);

			in.getline(buf,buf_size,',');
			daily.high = atof(buf);

			in.getline(buf,buf_size,',');
			daily.low = atof(buf);

			in.getline(buf,buf_size,',');
			daily.close = atof(buf);

			in.getline(buf,buf_size,',');
			daily.volume = atoll(buf);

			in.getline(buf,buf_size,'\n');
			daily.adj_close = atof(buf);

			timeseries.push_back( daily );
		}

		std::reverse( timeseries.begin(), timeseries.end() );
		return timeseries;
	}
	
	struct StockSplit {
		Gregorian::Date date;
		double factor;
	};

	std::vector<StockSplit> loadSplits( const char* file ) {
		std::vector<StockSplit> splits;
		std::ifstream in(file);
		if( !in ) return splits;

		const int buf_size = 16;
		char buf[buf_size];

		// skip over the header row
		//in.ignore( std::numeric_limits<std::streamsize>::max(), '\n');

		while( !(in.peek(),in.eof()) ) {
			StockSplit split;
			
			in.getline(buf,buf_size,',');
			split.date = parseDate( buf );

			in.getline(buf,buf_size,'\n');
			split.factor = atof(buf);

			splits.push_back( split );
		}

		std::reverse( splits.begin(), splits.end() );
		return splits;
	}

	struct Dividend {
		Gregorian::Date date;
		double amountPerShare;
	};

	std::vector<Dividend> loadDividends( const char* file ) {
		std::vector<Dividend> divs;
		std::ifstream in(file);
		if( !in ) return divs;

		const int buf_size = 16;
		char buf[buf_size];

		// skip over the header row
		//in.ignore( std::numeric_limits<std::streamsize>::max(), '\n');

		while( !(in.peek(),in.eof()) ) {
			Dividend div;
			
			in.getline(buf,buf_size,',');
			div.date = parseDate( buf );

			in.getline(buf,buf_size,'\n');
			div.amountPerShare = atof(buf);

			divs.push_back( div );
		}

		std::reverse( divs.begin(), divs.end() );
		return divs;
	}

	std::vector<DailyData> loadWithSplits( const char* file ) {
		auto timeseries = load(file);
		std::string f = file;
		f.pop_back(); f.pop_back(); f.pop_back(); f.pop_back();
		auto splits = loadSplits((f + "-splits.csv").c_str());
		double factor = 1.0;
		auto p = splits.begin();
		for( auto&& x : timeseries ) {
			if( p != splits.end() ) {
				if( x.date == p->date ) {
					factor *= p->factor;
					++p;
				}
			}
			x.open      *= factor;
			x.high      *= factor;
			x.low       *= factor;
			x.close     *= factor;
			x.adj_close *= factor;
		}
		return timeseries;
	}

}
}

#endif
