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

	struct DailyData {
		Gregorian::Date date;
		double open, high, low, close, adj_close;
		uint64_t volume;
	};

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

}
}

#endif
