/*
Bradley Bauer
Section 14
3-10-18
Project 7
*/

#include <iostream>
using std::cout; using std::endl;
#include <set>
using std::set;
#include <string>
using std::string;
using std::getline;
#include <stdexcept>
using std::invalid_argument;
using std::domain_error;
#include <fstream>
using std::ifstream;
#include <sstream>
using std::istringstream;
#include <iterator>
using std::ostream_iterator;
using std::inserter;
using std::next;
#include <algorithm>
using std::set_symmetric_difference;
using std::copy_n;

#include "proj07_functions.h"

/*
Connects the user to the server.
Inputs:
  sd (ServerData): current server state
  un (UserName): user to add
  sn (ServerName): server to add un to
Output:
  true if un was not in sn
  false if un was in sn
*/
bool AddConnection(ServerData &sd, ServerName sn, UserName un) {
	return sd[sn].insert(un).second;
}

/*
Removes the user from the server.
Inputs:
  sd (ServerData): current server state
  un (UserName): user to remove
  sn (ServerName): server to remove un from
Output:
  true if un was in sn
  false if un was not in sn
*/
bool DeleteConnection(ServerData &sd, ServerName sn, UserName un) {
	return sd[sn].erase(un);
}

/*
Builds a ServerData data structure from the file contents
Will throw if file cannot be opened or the file contains invalid commands
Inputs:
  fname (string &): file name to load data from
Output:
  sd (ServerData): ServerData data structure
*/
ServerData ParseServerData(const string &fname) {
	ServerData sd;
	ifstream file(fname);
	if (!file.is_open()) {
		throw invalid_argument("Failed to open file: " + fname);
	}
	string line;
	while (getline(file, line)) {
		istringstream line_stream(line);
		string user, command, server;
		line_stream >> user >> command >> server;

		if (command == "join") {
			sd[server].insert(user);
		}
		else if (command == "leave") {
			sd[server].erase(user);
		}
		else {
			throw domain_error("Invalid command: " + command);
		}
	}
	return sd;
}

/*
Pretty prints the ServerData to the out stream
Inputs:
  out (ostream&): the ostream to insert into
  sd (ServerData&): the ServerData do print
*/
void PrintAll(std::ostream &out, const ServerData &sd) {
	for (const auto &s : sd) {
		out << s.first << " : ";
		copy(s.second.begin(), --s.second.end(), ostream_iterator<string>(out, " "));
		out << *--s.second.end() << endl; // don't put a space after the last element
	}
}

/*
Returns the set of names of all servers in the ServerData data structure
Inputs:
  sd (ServerData&): the ServerData to get data from
Output:
  ret (set<string>): the set of all server names in sd
*/
set<string> AllServers(const ServerData &sd) {
	set<string> ret;
	for (const auto &server : sd) {
		ret.insert(server.first);
	}
	return ret;
}

/*
Returns the set of names of all users in the ServerData data structure
Inputs:
  sd (ServerData&): the ServerData to get data from
Output:
  ret (set<string>): the set of all user names in sd
*/
set<string> AllUsers(const ServerData &sd) {
	set<string> ret;
	for (const auto &server : sd) {
		for (const auto &user : server.second) {
			ret.insert(user);
		}
	}
	return ret;
}

/*
Returns the set of names of all servers that the provided user is connected to
Inputs:
  sd (ServerData&): the ServerData to get data from
  un (ServerName): the user to get server names for
Output:
  ret (set<string>): the set of all servers that un is connected to
*/
set<string> HasConnections(const ServerData &sd, UserName un) {
	set<string> ret;
	for (const auto &server : sd) {
		if (server.second.count(un)) {
			ret.insert(server.first);
		}
	}
	return ret;
}

/*
Returns the set of user names that are connected to the provided server
Inputs:
  sd (ServerData&): the ServerData to get data from
  sn (ServerName): the server to user names from
Output:
  ret (set<string>): the set of all users that are connected to sn
*/
set<string> HasUsers(const ServerData &sd, ServerName sn) {
	return sd.at(sn);
}

/*
Balances two servers so they have nearly the same number of users
Inputs:
  sd (ServerData&): the ServerData to get data from
  sn1 (ServerName): the first server to balance
  sn2 (ServerName): the first server to balance
Output:
  mutates sd[sn1] and sd[sn2] with new sets of users
*/
void BalanceServers(ServerData &sd, ServerName sn1, ServerName sn2) {
	set<string> &s1 = sd[sn1];
	set<string> &s2 = sd[sn2];
	set<string> unique;
	set_symmetric_difference(s1.begin(), s1.end(),
	                         s2.begin(), s2.end(),
	                         inserter(unique, unique.begin()));
	for (const auto &user : unique) {
		DeleteConnection(sd, sn1, user);
		DeleteConnection(sd, sn2, user);
	} // only balance with users not in both servers
	const size_t sz = unique.size();
	copy_n(unique.begin(),
	       sz / 2 + sz % 2,
	       inserter(s1, s1.begin())); // put n/2 + n%2 users in s1
	copy_n(next(unique.begin(), sz / 2 + sz % 2),
	       sz / 2,
	       inserter(s2, s2.begin())); // put n/2 users in s2
}

/*
Disconnects all users from all servers and then reconnects the users in a round-robin fashion
Inputs:
  sd (ServerData&): the ServerData to get data from
Output:
  mutates sd such that each server is balanced and no user is connected to more than one server
*/
void CleanAndBalance(ServerData &sd) {
	const set<string> &servers = AllServers(sd);
	const set<string> &users = AllUsers(sd);
	const int sz = servers.size();
	ServerData new_sd;
	int s = 0;
	for (const auto &user : users) {
		const auto &server = *next(servers.begin(), s++ % sz);
		AddConnection(new_sd, server, user);
	}
	sd = new_sd;
}