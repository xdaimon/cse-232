// name: Bradley Bauer
// section: 14
#include <iostream>
#include <iomanip>
#include <cmath>
using std::cout;
using std::cin;
using std::endl;
using std::fixed;
using std::setprecision;
using std::pow;

int main() {
    double car_price;
    double sales_tax;
    double downpayment;
    double title_and_fees;
    double yearly_interest_rate;
    double num_months;

    cin >> car_price;
    cin >> sales_tax;
    cin >> downpayment;
    cin >> title_and_fees;
    cin >> yearly_interest_rate;
    cin >> num_months;

    double monthly_rate = yearly_interest_rate / 12.;
    double final_cost_wo_downpayment = car_price * (1. + sales_tax) + title_and_fees - downpayment;
    double monthly_payment = final_cost_wo_downpayment * (monthly_rate * pow(1. + monthly_rate, num_months) / (pow(1. + monthly_rate, num_months) - 1.));

    cout << fixed << setprecision(2);
    cout << monthly_payment << endl;

    return 0;
}
