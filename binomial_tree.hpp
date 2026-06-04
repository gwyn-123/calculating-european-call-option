/*
Calculating European Call Option Pricing Using binomial tree
***************************************
Parameters:
S is the current stock price or spot price.
K is the exercise or strike price.
σ is the standard deviation of continuously compounded annual returns of the stock, 
which is called volatility.
T is the time for the option to expire in years.
r is the annualized risk-free interest rate
n is the number of time steps in the model
*/


#ifndef BINOMIAL_HPP
#define BINOMIAL_HPP

#include <cmath>
#include <vector>

using mat = std::vector<std::vector<double>>;

// Deterministic limit of a European call when volatility is zero/almost zero.
double deterministic_call_price(double S, double K, double T, double r) {
    return std::max(S - K * std::exp(-r * T), 0.0);
}

mat build_stock_tree(double S, int n, double T, double sd){

    double dt = T / n; // duration of each time step
    double u = exp(sd * sqrt(dt)); // up factor
    double d = 1/ u; // down factor

    mat stock_tree(n + 1, std::vector<double>(n + 1,0.0));

    stock_tree[0][0] = S;


    for(int i = 1; i < n; i ++){

        // Calculates the highest value node at time i
        stock_tree[0][i] = stock_tree[0][i-1] * u;

        for(int j = 1; j <= i; j ++){
            // Here we take advantage of the fact that the "order of ups and downs" doesn't matter, so every node after the "strictly increasing node" is the result of a previous node going down
            stock_tree[j][i] = stock_tree[j-1][i-1] * d;
        }
    }
    return stock_tree;
}

//this calculates the option value at each node of the stock tree
// This calculates the option value at each node of the stock tree.
mat binomial_values(mat stock_tree, int n, double T, double sd, double r, double K, double S) {

    double dt = T / static_cast<double>(n); // duration of each time step
    double u = std::exp(sd * std::sqrt(dt)); // up factor per time step
    double d = 1.0 / u; // down factor per time step

    double denominator = u - d;

    // Create the full value tree first, so every return path has the same type.
    mat option_values(n + 1, std::vector<double>(n + 1, 0.0));

    // Guard against near-zero volatility.
    // Otherwise u and d are almost equal, so:
    //
    // p = (exp(r * dt) - d) / (u - d)
    //
    // can explode because u - d is tiny.
    if (sd < 1e-4 || std::abs(denominator) < 1e-12) {
        option_values[0][0] = deterministic_call_price(S, K, T, r);
        return option_values;
    }

    double p = (std::exp(r * dt) - d) / denominator; // risk-neutral probability of price going up

    // If p is outside [0, 1], this CRR binomial tree is invalid for this row.
    // This can happen when volatility is too small relative to r and dt.
    if (p < 0.0 || p > 1.0) {
        option_values[0][0] = deterministic_call_price(S, K, T, r);
        return option_values;
    }

    double discount_rate = std::exp(-r * dt);

    // Calculates the option values at the final nodes.
    // This is just the payoff of the option at expiry.
    for (int j = 0; j <= n; j++) {
        option_values[j][n] = std::max(stock_tree[j][n] - K, 0.0);
    }

    // Calculates the option value at each previous node,
    // starting from time n - 1 and going back to time 0.
    for (int j = n - 1; j >= 0; j--) {
        for (int i = 0; i <= j; i++) {
            option_values[i][j] =
                discount_rate * (
                    p * option_values[i][j + 1]
                    + (1.0 - p) * option_values[i + 1][j + 1]
                );
        }
    }

    return option_values;
}

double binomial_option_price(double S, double K, double T, double r, double sd, int n) {
    if (S <= 0.0 || K <= 0.0 || T <= 0.0 || n <= 0) {
        return 0.0;
    }

    mat stock_tree = build_stock_tree(S, n, T, sd);
    mat option_values = binomial_values(stock_tree, n, T, sd, r, K, S);

    return option_values[0][0];
};

#endif