/*
Bradley Bauer
Section 14
2-24-18
Project 5
*/

#include <iostream>
using std::cout; using std::endl;
#include <string>
using std::string;
#include <vector>
using std::vector;
#include <cctype> // tolower, ...

const string alphabet("abcdefghijklmnoprstuvwxyz");

/*
Returns a lowercased version of s with non alphabetical characters removed
Inputs:
	s (string): the string to clean
Outputs:
	clean (string): the cleaned string
*/
string clean_string(string s) {
	string clean;
	for (char c : s) {
		if (isalpha(c)) {
			clean.push_back(tolower(c));
		}
	}
	return clean;
}

/*
Creates a string of length 25, that begins with the characters in key, and that contains only unique alphabetical characters
Inputs:
	key (string): the key to use in making an encoding
Outputs:
	encoding (string): the encoding
*/
string create_encoding(string key) {
	string encoding = alphabet;
	string prefix;
	for (char c : key) {
		string::size_type c_index = encoding.find(c);
		if (c_index != string::npos) {
			encoding.erase(c_index, 1);
			prefix += c;
		}
	}
	encoding = prefix + encoding;
	return encoding;
}

/*
Encodes a pair of characters using the four square cipher
Inputs:
	dg (string): the pair of characters to encode
	block1 (string): the first encoding block
	block2 (string): the second encoding block
Outputs:
	encoded (string): the encoded version of dg
*/
string encode_digraph(string dg, string block1, string block2) {
	string encoded;

	string::size_type i0 = alphabet.find(dg[0]);
	string::size_type i3 = alphabet.find(dg[1]);

	string::size_type row = i0 / 5;
	string::size_type col = i3 % 5;
	encoded.push_back(block1[row * 5 + col]);

	row = i3 / 5;
	col = i0 % 5;
	encoded.push_back(block2[row * 5 + col]);

	return encoded;
}

/*
Decodes a pair of characters that were encoded using the four square cipher
Inputs:
	dg (string): the pair of characters to decode
	block1 (string): the first encoding block
	block2 (string): the second encoding block
Outputs:
	decoded (string): the decoded version of dg
*/
string decode_digraph(string dg, string block1, string block2) {
	string decoded;

	string::size_type i1 = block1.find(dg[0]);
	string::size_type i2 = block2.find(dg[1]);

	string::size_type orig_index = i1 / 5 * 5 + i2 % 5;
	decoded.push_back(alphabet[orig_index]);

	orig_index = i2 / 5 * 5 + i1 % 5;
	decoded.push_back(alphabet[orig_index]);

	return decoded;
}

/*
Get pairs of characters from s
Inputs:
	s (string): the string to get pairs from
Outputs:
	pairs (vector<string>): the list of pairs
*/
vector<string> break_into_pairs(string s) {
	if (s.size() % 2) {
		cout << "Cannot make set of pairs from string with odd length." << endl;
	}
	vector<string> pairs;
	while (s.size()) {
		pairs.push_back(s.substr(0, 2));
		s = s.substr(2);
	}
	return pairs;
}

/*
Encodes a string using the four square cipher
Inputs:
	msg (string): the message to encode
	key1 (string): a string used to make an encoding
	key2 (string): a string used to make an encoding
Outputs:
	encoded (string): the encoded version of msg
*/
string encode(string msg, string key1, string key2) {
	string encoded;

	msg = clean_string(msg);
	if (msg.size() % 2) {
		msg.push_back('x');
	}

	vector<string> digraphs = break_into_pairs(msg);

	string block1 = create_encoding(key1);
	string block2 = create_encoding(key2);
	for (const string& di : digraphs) {
		encoded.append(encode_digraph(di, block1, block2));
	}

	return encoded;
}

/*
Decode a string that was encoded using the four square cipher
Inputs:
	msg (string): the encoded message to decode
	key1 (string): a key that was used to encode the message
	key1 (string): a key that was used to encode the message
Outputs:
	decoded (string): the original string
*/
string decode(string msg, string key1, string key2) {
	string decoded;

	vector<string> digraphs = break_into_pairs(msg);

	string block1 = create_encoding(key1);
	string block2 = create_encoding(key2);
	for (const string& p : digraphs) {
		decoded.append(decode_digraph(p, block1, block2));
	}

	return decoded;
}