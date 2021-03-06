// Bradley Bauer
// section 14
// 01 30 2018
// proj 03

#include<iostream>
using std::cout; using std::cin; using std::endl; using std::boolalpha;
#include<cmath>
using std::sqrt;
#include<string>
using std::to_string; using std::string;

string abIndex_friend(long ab_numerator, long ab_denominator, long f_pair){
  return to_string(ab_numerator) + "/" + to_string(ab_denominator) +
    ":" + to_string(f_pair);
}

long divisor_sum(long n) {
    // n == (n/d) * d + (n%d)
    // if (n%d) == 0, then (n/d) is a divisor and d is a divisor
    // by checking each d in [1, sqrt(n)], we find every divisor from [1, n]

    long sum = 0;
    long _sqrt = long(sqrt(n));
    for (long d = 1; d <= _sqrt; ++d)
        if (n % d == 0)
            sum += d + n / d;
    if (_sqrt*_sqrt == n) // prevent double counting when n == d*d
        sum -= _sqrt;
    return sum;
}

long gcd(long a, long b) {
    long r = 1;
    // if r = a%b divides b, then r divides a
    // b = qr
    // a = pb + a%b
    // a = pqr + r
    // a = r(pq + 1)
    // but since (pq + 1) and q are coprime, r is the gcd of a and b
    while (r) {
        r = a % b;
        a = b;
        b = r;
    }
    return a;

    /*
    long min = a;
    long max = b;
    while (max) {
        if (min > max) { // swap min and max
            long temp = min;
            min = max;
            max = temp;
        }
        max -= min; // gcd(min, max) == gcd(min, max-min)
    }
    return min;
    */

    /*
    if (max == min)
        return min;
    return gcd(min, max - min);
    */
}

bool is_solitary(long n) {
    if (n >= 1 && n <= 5)
        return true;
    return 1 == gcd(divisor_sum(n), n);
}

string friendly_check(long input, long limit) {
    long ab_num, ab_dem; // abundancy index of input
    ab_num = divisor_sum(input);
    ab_dem = input;

    long f_pair = -1; // friendly pair
    for (long n = 2; n <= limit; ++n) {

        long nab_num, nab_dem; // abundancy index of n
        nab_num = divisor_sum(n);
        nab_dem = n;

        if (nab_num*ab_dem == nab_dem*ab_num)
            if (n != input) {
                f_pair = n;
                break;
            }
    }

    long _gcd = gcd(ab_num, ab_dem);
    ab_num /= _gcd;
    ab_dem /= _gcd;
    return abIndex_friend(ab_num, ab_dem, f_pair);
}

int main (){
  cout << boolalpha;   // print true or false for bools
  int test;
  cin >> test;
  switch (test) {
  case 1 : {   // divisor sum test
    long input;
    cin >> input;
    cout << divisor_sum(input) << endl;
    break;
  } // of case 1

  case 2:{    // gcd test
    long first, second;
    cin >> first >> second;
    cout << gcd(first, second) << endl;
    break;
  } // of case 2

  case 3: {    // is_solitary test
    long input;
    cin >> input;
    cout << is_solitary(input) << endl;
    break;
  } // of case 3

  case 4: {
    // friend check. Make sure the return value is the
    // result of calling abIndex_friend, a string!
    long input, limit;
    cin >> input >> limit;
    cout << friendly_check(input, limit) << endl;
    break;
  } // of case 4
    
  } // of switch
} // of main
