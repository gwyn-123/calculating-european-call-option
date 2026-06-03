/*
Calculating European Call Option Price using Monte Carlo method:
We will use the formula:
C = e^{-rT}E^{\mathbb{Q}}[max(S_T - K, 0)],
where:
C is the European Call Option price
S_T is the stock price at the expiry date
K is the strike price
r is the risk free interest rate
T is time to maturity
*/

/*
The Monte Carlo method estimated the expectation by the simulation:
Expectation \approx e^{-rT}\frac{1}{N}\sum_{i=1}^{N}max(S_{T}^{(i)} - K, 0)
, where S_{T}^{(i)} ​is the simulated stock price in the i-th simulation

The model being used is the Black-Scholes stock price model. 
Under the risk-neutral measure, the stock price at time T is simulated as:
S_{T}^{i} = S_0 exp((r - \frac{1}{2}\sigma^2)T + \sigma\sqrt{T}Z),
where Z \sim N(0,1), \sigma is volatility, S_0 is the current stock price
*/

#ifndef MONTE_CARLO_HPP
#define MONTE_CARLO_HPP

#include <cmath>
#include <random> // This is the library that will allow us to have access to normal distribution
#include <algorithm>    // Needed for the "max" function
using namespace std;

double monte_carlo_european_call_option_price(int no_of_sim,double S, double K, double T, double r, double sd){
    // Compute the deterministic part of a simulated strike price
    double S_i_deterministic = S * exp((r - 0.5 * pow(sd,2)) * T);
    
    double pay_off_sum = 0;

    //Create random device
    random_device rd{};
    mt19937 gen{rd()};

    //Declare Normal distribution Z \sim N(0,1)
    normal_distribution d{0.0,1.0};

    //Calculate the simulated stock price at time T then calculate the expectation:
    for (int i = 0; i < no_of_sim; i += 1){
        double S_i_after_adjustment = S_i_deterministic * exp(sd * sqrt(T) * d(gen));
        pay_off_sum += max(S_i_after_adjustment - K, 0.0);
    };
    double expectation = pay_off_sum / static_cast<double>(no_of_sim);
    
    //The European Call Option Price is:
    double C = exp(-r * T) * expectation;
    return C;
}
#endif