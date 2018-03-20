// Bradley Bauer
// section 14
#include <iostream>
#include <cmath>

using std::cout;
using std::cin;
using std::endl;
using std::floor;
using std::pow;

int main() {
    int lower, upper;
    bool print;
    cin >> lower >> upper >> print;

    if (lower < 2 || upper < 2 || lower >= upper) {
        cout << "Error" << endl;
        return 0;
    }

    long max_ak = 0;
    int max_ak_i = 0;
    long max_length = 0;
    int max_length_i = 0;
    for (int i = lower; i <= upper; ++i) {
        long ak = i;
        long length = 0;

        if (print)
            cout << i << ": ";

        while (ak != 1) {

            if (ak > max_ak) {
                max_ak = ak;
                max_ak_i = i;
            }

            // conversion to long removes the fractional part
            ak = pow(ak, .5 + ak % 2);

            if (print) {
                cout << ak;
                if (ak != 1)
                    cout << ',';
            }

            ++length;
        }
        if (print)
            cout << endl;

        if (length > max_length) {
            max_length = length;
            max_length_i = i;
        }
    }
    cout << max_length_i << ", " << max_length << endl;
    cout << max_ak_i << ", " << max_ak << endl;

    return 0;
}
