/*
Calculating European Call Option Pricing Using Black - Scholes equation
Black - Scholes equation:
The price of a call option C in terms of the Black–Scholes parameters is
C = N(d_1) × S − N(d_2) × PV(K),
where:
S is the current stock price or spot price.
K is the exercise or strike price.
σ is the standard deviation of continuously compounded annual returns of the stock, 
which is called volatility.
T is the time for the option to expire in years.
r is the annualized risk-free interest rate
d_1 = 1/(σ * sqrt(T)) * (log(S/K) + (r + pow(σ,2)/2)*T)
d_2 = d_1 - σ * sqrt(T)
PV(K) = K * exp(-r * T)
N(d) = the integration from -∞ to d of the function 1/sqrt(2*M_PI) * exp(-pow(t, 2)/2) with 
regards to dt
*/

#ifndef BLACK_SCHOLES_HPP
#define BLACK_SCHOLES_HPP

#define _USE_MATH_DEFINES
#include <cmath>

using namespace std;

// Calculating the height of the probability density function at x in standard normal distribution:
double norm_pdf(double x){
    return  1/sqrt(2*M_PI) * exp(-pow(x, 2)/2);
}

/*
Calculating the value of the integration N(d), using the Abramowitz and Stegun approximation
of the CDF of the standard normal function using polynomial:
For x >= 0:
big_phi(x) = 1- small_phi(x)(0.319381530*k − 0.356563782*pow(k,2) + 1.781477937*pow(k,3)
            −1.821255978*pow(k,4) + 1.330274429*pow(k,5))

For x < 0:
big_phi(x) = 1 - big_phi(-x)
note: small_phi(x) is the height of the PDF at x
k = 1/(1 + 0.2316419*x)
*/
double norm_cdf(double y){
    double k = 1/(1 + 0.2316419*y);
    double sum_k = 0.319381530*k - 0.356563782*pow(k,2) + 1.781477937*pow(k,3) - 1.821255978*pow(k,4) + 1.330274429*pow(k,5);
    if (y >= 0){
        return 1 - norm_pdf(y) * sum_k;
    }
    else {
        return 1.0 - norm_cdf(-y);
    };
}

// Calcualting d_1 and d_2 value in general (d_j)
double d_j(int j, double S, double K, double T, double r, double sd){
    double d_1 = 1/(sd * sqrt(T)) * (log(S/K) + (r + pow(sd,2)/2)*T);
    double d_2 = d_1 - sd * sqrt(T);
    return (2 - j) * d_1 + (j - 1) * d_2;
    // sd is the standard deviation or σ in the above note
}

// Calculating the European Call Option price:
double european_call_option_price(double S, double K, double T, double r, double sd){
    double PV_K = K* exp(-r * T);
    return norm_cdf(d_j(1, S, K, T, r, sd)) * S - norm_cdf(d_j(2,S,K,T,r,sd)) * PV_K; 
}

#endif