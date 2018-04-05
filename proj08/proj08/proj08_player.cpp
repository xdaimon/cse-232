/*
Bradley Bauer
Section 14
03-18-18
Project 8

This module implements the Player's functions.
The Player class keeps track of a stock market player's cash and number of stocks.
The Player class can buy and sell stocks and dump the player's information to a string.
*/

#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <iomanip>

#include "proj08_player.h"

/*
Buys a quantity of stock on given date
Inputs
    m (Market&): the stock market data structure
    stock (string): the stock to buy
    date (long): the date to buy the stocks on
Outputs
    (bool): whether stocks were purchased
*/
bool Player::buy(Market &m, string stock, long date, long quantity) {
    // If stock and date are valid
    double price = m.get_price(stock, date);
    if (price < 0.) {
        return false;
    }

    // and if player has enough cash
    if (price * quantity > cash) {
        return false;
    }

    // then buy the stock and update the player info
    cash -= quantity * price;
    stocks[stock] += quantity;
    return true;
}

/*
Sells a quantity of stock on given date if stock and date are valid and if user has enough quantity
Inputs
    m (Market&): the stock market data structure
    stock (string): the stock to sell
    date (long): the date to sell the stocks on
Outputs
    (bool): whether stocks were sold given the conditions above
*/
bool Player::sell(Market &m, string stock, long date, long quantity) {
    // If player has stock
    auto stock_ptr = stocks.find(stock);
    if (stock_ptr == stocks.end()) {
        return false;
    }

    // and market also has stock
    double price = m.get_price(stock, date);
    if (price < 0.) {
        return false;
    }

    // and user has enough to sell
    long &stock_owned = stock_ptr->second;
    if (quantity > stock_owned) {
        return false;
    }

    // then sell the stock and update the player info
    cash += quantity * price;
    stock_owned -= quantity;
    if (stock_owned == 0) {
        stocks.erase(stock);
    }
    return true;
}

/*
Converts the player data structure to a string
Outputs
    (string) a string containing all the stock/quantity/cash info contained in player
*/
string Player::to_str() {
    // Only show cash as dollars and cents
    ostringstream oss;
    oss << std::setprecision(2) << std::fixed;
    oss << cash;

    // Put "stocks:quantity" into string
    for (const auto &p : stocks) {
        oss << "," << p.first << ":" << p.second;
    }
    return oss.str();
}

/*
Return the combination of two player data structures and set the input players to zero
Inputs
    p2 (Player&): player to combine with *this
Outpus
    player (Player): the combined player
*/
Player Player::combine(Player & p2) {
    // player is a copy of *this
    Player player = *this;

    // combine cash and stocks
    player.cash += p2.cash;
    for (const auto &p : p2.stocks) {
        player.stocks[p.first] += p.second;
    }

    // zero the to given players
    *this = Player(0);
    p2 = Player(0);
    return player;
}
