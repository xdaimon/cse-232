/*
Bradley Bauer
Section 14
03-18-18
Project 8
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

Market::Market(string filename) {
	ifstream file(filename);
	istringstream stream;
	string line;
	while (getline(file, line)) {
		stream.str(line); // does not clear eof bit
		long date;
		stream >> date;
		copy_n(istream_iterator<double>(stream), 30, back_inserter(stocks[date]));
		stream.clear(); // copy's last extraction set the stream's eof bit
	}
}

double Market::get_price(string symbol, long date) const {
	auto price_index = find(symbol_list.begin(), symbol_list.end(), symbol) - symbol_list.begin();
	if (price_index != symbol_list.size()) { // if database has data for symbol
		if (stocks.count(date)) { // if database has data for date
			return stocks.at(date)[price_index];
		}
	}
	return -1.0;
}

pair<double, double> Market::high_low_year(long year, string symbol) {
	// the symbol's index in symbol_list is the same as the symbol's price in the price vectors
	auto price_index = find(symbol_list.begin(), symbol_list.end(), symbol) - symbol_list.begin();
	if (price_index != symbol_list.size()) { // if database has data for symbol
		// use binary search to find the exact range of years to process
		auto first = stocks.lower_bound(year*10000); // points to min x s.t. x >= year
		auto end = stocks.lower_bound((year+1)*10000);
		if (distance(first, end) > 0) { // if database has data for date
			double min = numeric_limits<double>::min();
			double max = numeric_limits<double>::max();
			auto high_low = pair<double, double>{min, max};
			for_each(first, end, [&](const pair<long, vector<double>> &p) {
				high_low.first = std::max(p.second[price_index], high_low.first);
				high_low.second = std::min(p.second[price_index], high_low.second);
			});
			return high_low;
		}
	}
	return {-1.0, -1.0};
}
