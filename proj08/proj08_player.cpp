/*
Bradley Bauer
Section 14
03-18-18
Project 8
*/

#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <iomanip>

#include "proj08_player.h"

bool Player::buy(Market &m, string stock, long date, long quantity) {
	double price = m.get_price(stock, date);
	if (price >= 0.) {
		if (cash >= quantity * price) {
			cash -= quantity * price;
			stocks[stock] += quantity;
			return true;
		}
	}
	return false;
}

bool Player::sell(Market &m, string stock, long date, long quantity) {
	auto stock_ptr = stocks.find(stock);
	if (stock_ptr != stocks.end()) {
		auto &stock_val = stock_ptr->second;
		if (quantity <= stock_val) {
			double price = m.get_price(stock, date);
			if (price >= 0.) {
				cash += quantity * price;
				stock_val -= quantity;
				if (stock_val == 0) {
					stocks.erase(stock);
				}
				return true;
			}
		}
	}
	return false;
}

string Player::to_str() {
	ostringstream oss;
	oss << std::setprecision(2) << std::fixed;
	oss << cash;
	for (auto &p : stocks) {
		oss << "," << p.first << ":" << p.second;
	}
	return oss.str();
}

Player Player::combine(Player & p2) {
	Player player = *this;
	player.cash += p2.cash;
	for (const auto &p : p2.stocks) {
		player.stocks[p.first] += p.second;
	}
	*this = Player(0);
	p2 = Player(0);
	return player;
}
