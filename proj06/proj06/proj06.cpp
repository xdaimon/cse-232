/*
Bradley Bauer
Section 14
2-24-18
Project 6
*/

#include <iostream>
using std::cout; using std::endl;
#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <vector>
using std::vector;
#include <iterator>
using std::ostream_iterator;
#include <algorithm>
using std::copy;

/*
Converts a vector to a string where elements are separated by a comma
Inputs:
	v (const vector<long>&): the vector to stringify
Outputs:
	representation (string): the string representation of the vector
*/
string vec_2_str(const vector<long>& v) {
	ostringstream oss;
	copy(v.begin(), v.end()-1, ostream_iterator<long>(oss, ","));
	oss << v.back();
	return oss.str();
}

/*
Compute an nstep sequence and return it in a vector.
Inputs:
	limit (long): all numbers in the nstep vector are <= than limit
	nstep (long): the number of previous terms used to sum to compute the next term
Outputs:
	sequence (vector<long>): the nstep vector
*/
vector<long> gen_nstep_vector(long limit, long nstep) {
	// f(n) = 0 for n < 1,
	// f(1) = 1,
	// f(2) = 1,
	// f(n) = 2*f(n-1) - f(n-nstep), for n > 2
	vector<long> sequence{ 1 };
	long size = 1;
	long next = 1;
	while (next <= limit) {
		// Push back the nth element
		sequence.push_back(next);
		++size;

		// Compute the (n+1)th element
		next = 2 * next;
		if (size > nstep) { // better than a nested loop -> only 1 condition check, only 1 dereferrence
			next -= sequence[size - 1 - nstep];
		}
	}
	return sequence;
}

/*
Encode a number into binary based on a fibonacci n-step sequence
Inputs:
	num (long): the number to code
	nstep (long): the n-step in 'fibonacci n-step sequence'
Outputs:
	coded (string): a binary string
*/
#include <numeric>
string num_to_nstep_coding(long num, long nstep) {
	string coded;
	vector<long> nstep_seq = gen_nstep_vector(num, nstep);
	for (size_t i = nstep_seq.size() - 1; i >= 1; --i) {
		bool lt = nstep_seq[i] <= num;
		if (lt) {
			num -= nstep_seq[i];
		}
		coded += '0' + lt;
	}
	std::reverse(coded.begin(), coded.end());
	return coded;
}

/*
Convert num back from its nstep encoding
Inputs:
	coding (const string&): a binary string
	nstep_sequence (const vector<long>&): the nstep sequence
Outputs:
	decoded (long): the long whose fibonacci coding was the input coding
*/
long nstep_coding_to_num(const string & coding, const vector<long>& nstep_sequence) {
	long decoded = 0;
	string::size_type next_index = coding.find('1');
	while (next_index != string::npos) {
		// the ith bit in coding corresponds to the coefficient on the (i+1)th term in sum(nstep_sequence)
		decoded += nstep_sequence[next_index + 1];
		next_index = coding.find('1', next_index + 1);
	}
	return decoded;
}
int main() {
	cout << num_to_nstep_coding(100, 2) << endl;
}
