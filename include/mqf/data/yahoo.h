#ifndef INCLUDED_MQF_DATA_YAHOO
#define INCLUDED_MQF_DATA_YAHOO
#include <fstream>
#include "stocks.h"
#include <cstring>

/*
 * Parsing stock price data from Yahoo Finance
 *
 */
namespace mqf {
namespace Yahoo {

	/*
	 * Parse date of the form "yyyymmdd", e.g. "20150427"
	 *
	 */
	Gregorian::Date parseDate( const char* str ) {
		Gregorian::Date date;
		
		char temp[11];

		temp[0] = str[0];
		temp[1] = str[1];
		temp[2] = str[2];
		temp[3] = str[3];
		temp[4] = '\0';
		temp[5] = str[4];
		temp[6] = str[5];
		temp[7] = '\0';
		temp[8] = str[6];
		temp[9] = str[7];
		temp[10] = '\0';

		date.year = atoi( &temp[0] );
		date.month = Gregorian::Month( atoi( &temp[5] ) );
		date.day = atoi( &temp[8] );

		return date;
	}

	/*
	 * Load csv files produced by real-chart.finance.yahoo.com/x
	 *
	 */
	StockData load( const char* file ) {
		std::ifstream in(file);
		if( !in ) return {};

		const int buf_size = 16;
		char buf[buf_size];

		auto smax = std::numeric_limits<std::streamsize>::max();

		StockData data;

		// skip over the header row
		in.ignore(smax,'\n');

		while( !(in.peek(),in.eof()) ) {

			in.getline(buf,buf_size,',');

			// if the first char is a number, assume daily price data
			if( buf[0] >= '0' && buf[0] <= '9' ) {
				
				auto date = parseDate( buf );

				in.getline(buf,buf_size,',');
				auto open = atof(buf);

				in.getline(buf,buf_size,',');
				auto high = atof(buf);

				in.getline(buf,buf_size,',');
				auto low = atof(buf);

				in.getline(buf,buf_size,',');
				auto close = atof(buf);

				in.getline(buf,buf_size,',');
				auto volume = atoll(buf);

				in.ignore(smax,'\n');

				data.dates .push_back( date   );
				data.open  .push_back( open   );
				data.high  .push_back( high   );
				data.low   .push_back( low    );
				data.close .push_back( close  );
				data.volume.push_back( volume );

			} else if( strcmp( buf, "DIVIDEND" ) == 0 ) {
				
				Dividend div;

				// ignore the single space
				in.ignore(1);

				in.getline(buf,buf_size,',');
				div.date = parseDate( buf );

				in.getline(buf,buf_size,'\n');
				div.amountPerShare = atof(buf);

				data.dividends.push_back( div );

			} else if( strcmp( buf, "SPLIT" ) == 0 ) {
				
				Split split;

				// ignore the single space
				in.ignore(1);

				in.getline(buf,buf_size,',');
				split.date = parseDate( buf );

				in.getline(buf,buf_size,':');
				auto numerator = atof(buf);

				in.getline(buf,buf_size,'\n');
				auto denominator = atof(buf);

				split.factor = numerator / denominator;

				data.splits.push_back( split );

			} else {
				in.ignore(smax,'\n');
			}
			
		}

		std::reverse( data.dates    .begin(), data.dates    .end() );
		std::reverse( data.open     .begin(), data.open     .end() );
		std::reverse( data.high     .begin(), data.high     .end() );
		std::reverse( data.low      .begin(), data.low      .end() );
		std::reverse( data.close    .begin(), data.close    .end() );
		std::reverse( data.volume   .begin(), data.volume   .end() );
		std::reverse( data.dividends.begin(), data.dividends.end() );
		std::reverse( data.splits   .begin(), data.splits   .end() );

		return data;
	}

}
}

#endif
