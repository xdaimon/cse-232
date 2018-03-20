// Bradley Bauer
// Section 14
// Project 4
// 2-3-18

#include<iostream>
using std::cout; using std::cin; using std::endl; using std::boolalpha;
#include<string>
using std::string;
#include<cmath>

const string the_chars = "0123456789abcdef";

// Reverses the input string
// Inputs:
//   s: (string) input to reverse
// Outputs:
//   ret: (string) reversed copy of s
string reverse_string(string s) {
    string ret;
    for (int i = s.size()-1; i >= 0; --i) {
        ret += s[i];
    }
    return ret;
}

// Checks if the input string is a palindrome
// Inputs:
//   s: (string) input to check
// Outputs:
//   (bool) whether s is a palindrome
bool is_palindrome(string s) {
    return reverse_string(s) == s;
}

// Represent a number as a string of digits in base b
// Inputs:
//   n: (long) number to represent in base b
//   b: (long) the base to encode n into
// Outputs:
//   ret: (string) the string representation of n in base b
string long_to_base(long n, long b) {
    string ret;
    while (n) {
        ret = the_chars[n % b] + ret;
        n /= b;
    }
    return ret;
}

// Checks if a number is prime
// Inputs:
//   n: (long) the number to check from primality
// Outputs:
//   (bool): whether n is prime
bool is_prime(long n) {
    if (n <= 1) {
        return false;
    }
    if (n == 2 || n == 3) {
        return true;
    }
    if (n % 2 == 0 || n % 3 == 0) {
        return false;
    }

    long n_sqrt = long(sqrt(n));

    // every prime > 3 can be written as 6x+1 or 6x-1 for an int x
    for (long i = 6; i <= n_sqrt+1; i += 6) {
        // so try to check as few composite factors as possible
        if (n % (i - 1) == 0 || n % (i + 1) == 0) {
            return false;
        }
    }

    return true;
}

// Check if a number is both prime and palindromic in binary, decimal, and hex
// Inputs:
//   n: (long) the number to check
// Outputs:
//   (string) a string representing if n is palindromic and prime in a certain base
string is_pal_prime(long n) {
    if (!is_prime(n)) {
        return "not-pal-prime";
    }

    string binary_n = long_to_base(n, 2);
    if (is_palindrome(binary_n)) {
        return "binary-pal-prime";
    }

    string decimal_n = long_to_base(n, 10);
    if (is_palindrome(decimal_n)) {
        return "decimal-pal-prime";
    }

    string hex_n = long_to_base(n, 16);
    if (is_palindrome(hex_n)) {
        return "hex-pal-prime";
    }

    return "not-pal-prime";
}

int main (){
  long test_num;
  cin >> test_num;
  cout << boolalpha;

  switch(test_num) {

  case 1: {
    string input;
    cin >> input;
    cout << reverse_string(input);
    break;
  }

  case 2:{
    string input;
    cin >> input;
    cout << is_palindrome(input);
    break;
  }

  case 3:{
    long n, base;
    cin >> n >> base;
    cout << long_to_base(n,base);
    break;
  }

  case 4:{
    long n;
    cin >> n;
    cout << is_prime(n);
    break;
  }

  case 5:{
    long n;
    cin >> n;
    cout << is_pal_prime(n);
    break;
  }

  } // of switch
}  // of main

