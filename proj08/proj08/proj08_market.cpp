/*
Bradley Bauer
Section 14
03-18-18
Project 8

This module implements the Market class functionality.
The Market contains a map between date and list of prices.
The list of price for each date corresponds the the price for each symbol in the
Market's symbol list. So, the price on date d for the ith symbol is map[d][i].
The Market allows users to get the price of a stock for a given date and also
allows users to get the max and min price of a given stock throughout a year.
*/

#include "proj08_market.h"

#include <string>
using std::string;
using std::getline;
#include <sstream>
using std::istringstream;
#include <iterator>
using std::istream_iterator;
using std::back_inserter;
using std::distance;
#include <limits>
using std::numeric_limits;
#include <fstream>
using std::ifstream;
#include <algorithm>
using std::copy_n;
using std::for_each;
using std::find;

/*
The Market constructor parses a file into the Market data structor
Inputs
    filename (string): the file that holds the stock market data
Outputs
    initializes Market's members
*/
Market::Market(string filename) {
    ifstream file(filename);
    string line;
    long date;
    while (getline(file, line)) {
        // each line of file contains a date (long) followed by 30 prices (float)
        istringstream stream(line);

        stream >> date;
        // creates empty vector, by map[key], and inserts 30 doubles into it
        copy_n(istream_iterator<double>(stream), 30, back_inserter(stocks[date]));
    }
}

/*
Returns the price of the stock on the given date if symbol and date are valid
Inputs
    symbol (string): the stock's symbol, in format "AB"
    date (long): the date, in format 20120101
Outputs
    (double): the dollar and cents price of stock on date
*/
double Market::get_price(string symbol, long date) const {
    // if database has data for date
    if (stocks.count(date) == 0) {
        return -1.;
    }

    // and if database has data for symbol
    auto price_index = find(symbol_list.begin(), symbol_list.end(), symbol) - symbol_list.begin();
    if (price_index == symbol_list.size()) {
        return -1.;
    }

    // then return the stock's price
    const vector<double> &prices = stocks.at(date);
    return prices[price_index];
}

/*
Returns the max and min prices that the stock took on throughout the year
Inputs
    year (long): the year of data to look at
    symbol (string): the symbol to find the high low price for
Outputs
    (pair<double,double>): the {high,low} price or {-1, -1} if symbol or year are not in the market
*/
pair<double, double> Market::high_low_year(long year, string symbol) {
    // if database has data for symbol

    // the symbol's index in symbol_list is the same as the symbol's index in the price vectors
    auto price_index = find(symbol_list.begin(), symbol_list.end(), symbol) - symbol_list.begin();
    if (price_index == symbol_list.size()) {
        return { -1.0, -1.0 };
    }

    // and if database has data for year

    // use binary search to find the exact range of dates to process
    auto first = stocks.lower_bound(year * 10000); // an iterator that points to the min x in stocks s.t. x.date >= year*10000
    auto end = stocks.lower_bound((year + 1) * 10000);
    if (distance(first, end) <= 0) {
        return { -1.0, -1.0 };
    }

    // then find min and max prices for the stock within the year
    double max = numeric_limits<double>::min();
    double min = numeric_limits<double>::max();
    for_each(first, end, [&](const pair<long, vector<double>> &p) {
        max = std::max(p.second[price_index], max);
        min = std::min(p.second[price_index], min);
    });
    return { max, min };
}
